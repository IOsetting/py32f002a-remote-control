#include <string.h>
#include <stdio.h>
#include "main.h"
#include "py32f0xx_bsp_clock.h"
#include "py32f0xx_msp.h"
#include "drv_wireless.h"
#include "drv_lspwm.h"
#include "drv_hspwm.h"
#include "drv_servo.h"
#include "drv_sys.h"

typedef enum
{
  STATE_PAIR, STATE_RUN
} STATE_T;

STATE_T device_state;

__IO uint16_t device_idle_counter;
/*
 * Use UID[4:11]
*/
uint8_t device_uuid[8];
/*
 * [0:5]: Analog channel 1 - 6
 * [6]:   Key 1 - 8
*/ 
uint8_t pad_state[7];

uint8_t servo_pwm_channel[8];

const uint8_t DIRECTION[4] = {0, 0, 0, 0};

const uint8_t PAIR_ADDR[5] = {0x11,0x33,0x33,0x33,0x11};

void APP_Init(void);
STATE_T APP_Pair(void);
STATE_T APP_Run(void);

int main(void)
{
  BSP_RCC_HSI_PLL48MConfig();

  DEBUG_PRINTF("Remote Control: Servo Driver\r\nClock: %ld\r\n", SystemCoreClock);

  /** 
   * Important: 
   * delay 2 seconds before SWD port stop working, so you will have 
   * enougth time to re-flash the MCU
  */
  LL_mDelay(2000);

  /* Check if PF0/PF2 pin has been set as GPIO pin*/
  if(READ_BIT(FLASH->OPTR, FLASH_OPTR_NRST_MODE) == OB_RESET_MODE_RESET)
  {
    DEBUG_PRINT_STRING("Write option bytes\r\n");
    /* If not, turn off the RESET function on pin(PF0/PF2), this will reset the MCU */
    MSP_FlashSetOptionBytes();
  }
  DEBUG_PRINT_STRING("Option bytes have been set\r\n");

  MSP_GPIO_Init();
  MSP_SPI_Init();
  MSP_TIM1_Config();
  MSP_TIM1_PWMConfig();
  MSP_TIM3_Config();
  MSP_TIM3_PWMConfig();
  MSP_TIM14_Config();

  APP_Init();

  device_state = STATE_RUN;
  device_idle_counter = 0;

  /* Infinite loop */
  while(1)
  {
    switch (device_state)
    {
    case STATE_RUN:
      device_state = APP_Run();
      break;

    case STATE_PAIR:
    default:
      device_state = APP_Pair();
      break;
    }
    LL_mDelay(3);
  }
}

void APP_Init(void)
{
  DRV_SYS_ReadDeviceUID((uint32_t *)device_uuid, 1, 2);
#if DEBUG == 1
  SEGGER_RTT_WriteString(0, "UUID: ");
  for (uint8_t i = 0; i < 8; i++)
  {
    SEGGER_RTT_printf(0, "%02X", *(device_uuid + i));
  }
  SEGGER_RTT_WriteString(0, "\r\n");
#endif

  while (DRV_Wireless_Test() == ERROR)
  {
    DEBUG_PRINT_STRING(" - check failed\r\n");
    LL_mDelay(1000);
  }
  DEBUG_PRINT_STRING(" - check passed\r\n");

  DRV_Wireless_Init(77, device_uuid, (uint8_t *)PAIR_ADDR);

  DRV_LSPWM_Init();
}

/**
 * In Pair state:
 * 1. switch to TX mode
 * 2. send its RX address to a fixed address
 * 3. switch to RX mode
 * 4. wait incoming control data
 * 5. if data is received, return RUN state
 * 6. if not return PAIR state
*/
STATE_T APP_Pair(void)
{
  uint8_t i, j, tx_data[8];
  static uint8_t rotate;

  DRV_Wireless_TxMode();
  DEBUG_PRINT_STRING(".");
  /* Use j to calculate the CRC */
  j = 0;
  for (i = 0; i < 7; i++)
  {
    tx_data[i] = device_uuid[i];
    j += device_uuid[i];
  }
  tx_data[7] = j;

  DRV_Wireless_Tx(tx_data);
  DRV_Wireless_RxMode();

  j = 128;
  while(j--)
  {
    if (DRV_Wireless_Rx(&i, pad_state) == SUCCESS)
    {
      DEBUG_PRINT_STRING("Data received. Enter RUN state\r\n");
      return STATE_RUN;
    }
    LL_mDelay(1);
  }
  if (++rotate == 0)
  {
    DEBUG_PRINT_STRING("\r\n");
  }
  return STATE_PAIR;
}

STATE_T APP_Run(void)
{
  uint8_t i;

  if (DRV_Wireless_Rx(&i, pad_state) == SUCCESS)
  {
    DEBUG_PRINTF("%03d,PIPE:%01d, %02X %02x %02x %02x %02x %02x %02X ", 
      device_idle_counter, i, 
      *pad_state, *(pad_state + 1), *(pad_state + 2), *(pad_state + 3), 
      *(pad_state + 4), *(pad_state + 5), *(pad_state + 6));
    // Keys
    for (i = 8; i--;)
    {
      if (*(pad_state + 6) & (1 << i))
      {
        DRV_LSPWM_SetDuty(i, 0xFF, 0xFF);
      }
      else
      {
        DRV_LSPWM_SetDuty(i, 0, 0xFF);
      }
    }
    // Convert Analog channels (X:A1, Y:A0, Z:A2) to PWM
    DRV_SERVO_AnalogConvert(*(pad_state + 1), *(pad_state), *(pad_state + 2), (uint8_t *)DIRECTION, servo_pwm_channel);
    // Update PWM channels
    for (i = 0; i < 8; i++)
    {
      DRV_LSPWM_SetDuty(8 + i, *(servo_pwm_channel + i), 0xFF);
    }
    // Update high speed PWM
    if ((*(pad_state + 6) & (1 << 0)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(1, -1);
      DRV_HSPWM_IncreaseDuty(2,  1);
    }
    if ((*(pad_state + 6) & (1 << 1)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(1,  1);
      DRV_HSPWM_IncreaseDuty(2, -1);
    }
    if ((*(pad_state + 6) & (1 << 2)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(3, -1);
      DRV_HSPWM_IncreaseDuty(4,  1);
    }
    if ((*(pad_state + 6) & (1 << 3)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(3,  1);
      DRV_HSPWM_IncreaseDuty(4, -1);
    }

    if ((*(pad_state + 6) & (1 << 4)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(5, -1);
      DRV_HSPWM_IncreaseDuty(6,  1);
    }
    if ((*(pad_state + 6) & (1 << 5)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(5,  1);
      DRV_HSPWM_IncreaseDuty(6, -1);
    }
    if ((*(pad_state + 6) & (1 << 6)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(7, -1);
      DRV_HSPWM_IncreaseDuty(8,  1);
    }
    if ((*(pad_state + 6) & (1 << 7)) == 0)
    {
      DRV_HSPWM_IncreaseDuty(7,  1);
      DRV_HSPWM_IncreaseDuty(8, -1);
    }

    device_idle_counter = 0;
  }
  // Enter Pair state after 5 seconds, device_pair_counter is updated by TIM14
  if (device_idle_counter > LSPWM_FREQUENCY * LSPWM_PERIOD * 5)
  {
    device_idle_counter = 0;
    DEBUG_PRINT_STRING("Enter PAIR state\r\n");
    return STATE_PAIR;
  }
  return STATE_RUN;
}


void APP_ErrorHandler(void)
{
  while (1);
}

#ifdef  USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
  while (1);
}
#endif /* USE_FULL_ASSERT */
