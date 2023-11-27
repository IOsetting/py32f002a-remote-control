#include <string.h>
#include <stdio.h>
#include "main.h"
#include "py32f0xx_bsp_clock.h"
#include "py32f0xx_msp.h"
#include "drv_wireless.h"
#include "drv_lspwm.h"
#include "drv_hspwm.h"
#include "drv_servo.h"

/*
 * [0:5]: Analog channel 1 - 6
 * [6]:   Key 1 - 8
*/ 
uint8_t pad_state[7];

uint8_t servo_pwm_channel[8];

const uint8_t DIRECTION[4] = {0, 0, 0, 0};

const uint8_t RX_ADDRESS[5] = {0x33,0x55,0x33,0x44,0x33};
const uint8_t TX_ADDRESS[5] = {0x11,0x33,0x33,0x33,0x11};

int main(void)
{
  uint16_t i = 0, j = 0;

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

  MSP_TIM14_Config();

  while (DRV_Wireless_Test() == ERROR)
  {
    DEBUG_PRINT_STRING(" - check failed\r\n");
    LL_mDelay(1000);
  }
  DEBUG_PRINT_STRING(" - check passed\r\n");

  DRV_Wireless_Init(78, (uint8_t *)RX_ADDRESS, (uint8_t *)TX_ADDRESS);

  DRV_LSPWM_Init();
  
  /* Infinite loop */
  while(1)
  {
    j++;
    if (DRV_Wireless_Rx(pad_state) == SUCCESS)
    {
#if DEBUG == 1
      SEGGER_RTT_printf(0, "%03d %02X %02x %02x %02x %02x %02x %02X ", 
        j, *pad_state, *(pad_state + 1), *(pad_state + 2), *(pad_state + 3), *(pad_state + 4), *(pad_state + 5), *(pad_state + 6));
#endif
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
        DRV_HSPWM_SetDutyCH1(1);
      }
      if ((*(pad_state + 6) & (1 << 1)) == 0)
      {
        DRV_HSPWM_SetDutyCH2(1);
      }
      if ((*(pad_state + 6) & (1 << 2)) == 0)
      {
        DRV_HSPWM_SetDutyCH3(1);
      }
      if ((*(pad_state + 6) & (1 << 3)) == 0)
      {
        DRV_HSPWM_SetDutyCH4(1);
      }

      j = 0;
    }

    LL_mDelay(10);
  }
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
