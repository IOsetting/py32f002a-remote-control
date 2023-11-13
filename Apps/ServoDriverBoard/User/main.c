#include <string.h>
#include <stdio.h>
#include "main.h"
#include "py32f0xx_bsp_clock.h"
#include "py32f0xx_msp.h"
#include "xl2400.h"
#include "drv_lsio.h"

/*
 * [0:5]: Analog channel 1 - 6
 * [6]:   Key 1 - 8
*/ 
uint8_t pad_state[7];

const uint8_t TX_ADDRESS[5] = {0x11,0x33,0x33,0x33,0x11};
const uint8_t RX_ADDRESS[5] = {0x33,0x55,0x33,0x44,0x33};

extern uint8_t xbuf[XL2400_PL_WIDTH_MAX + 1];

int main(void)
{
  uint16_t i = 0, j = 0;
  uint8_t crc;

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

  while (XL2400_SPI_Test() == ERROR)
  {
    DEBUG_PRINT_STRING(" - check failed\r\n");
    LL_mDelay(1000);
  }
  DEBUG_PRINT_STRING(" - check passed\r\n");

  XL2400_Init();
  XL2400_SetPower(XL2400_RF_0DB);
  XL2400_SetChannel(77);
  XL2400_SetTxAddress(RX_ADDRESS);
  XL2400_SetRxAddress(TX_ADDRESS);
  XL2400_WakeUp();
  XL2400_SetRxMode();

  DRV_LSIO_Init();
  
  /* Infinite loop */
  while(1)
  {
    j++;
    if (XL2400_Rx() & XL2400_FLAG_RX_DR)
    {
      SEGGER_RTT_printf(0, "%03d %02X %02x %02x %02x %02x %02x %02X %02X ", 
        j, *xbuf, *(xbuf + 1), *(xbuf + 2), *(xbuf + 3), *(xbuf + 4), *(xbuf + 5), *(xbuf + 6), *(xbuf + 7));
      // CRC check
      crc = 0;
      for (i = 0; i < XL2400_PLOAD_WIDTH - 1; i++)
      {
        crc += *(xbuf + i);
      }
      if (crc != *(xbuf + XL2400_PLOAD_WIDTH - 1))
      {
        SEGGER_RTT_WriteString(0, "CRC Error\r\n");
      }
      else
      {
        SEGGER_RTT_WriteString(0, "CRC OK\r\n");
        // Store received data
        memcpy(pad_state, xbuf, 7);
      }

      // Keys
      for (i = 8; i--;)
      {
        if (*(pad_state + 6) & (1 << i))
        {
          DRV_LSIO_SetDuty(i, PWM_PERIOD, 0xFF);
        }
        else
        {
          DRV_LSIO_SetDuty(i, 0, 0xFF);
        }
      }
      // Analog channels
      for (i = 0; i < 6; i++)
      {
        DRV_LSIO_SetDuty(8 + i, *(xbuf + i), 0xFF);
      }

      j = 0;
    }

    LL_mDelay(10);
  }
}

void APP_TIM1UpdateCallback(void)
{
  DRV_LSIO_Tick();
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
