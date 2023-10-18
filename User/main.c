/***
 * Demo: ST7567 LCD
 * 
 * PY32          ST7567
 * PB0   ------> Reset
 * PF0   ------> CLK/SCK
 * PB1   ------> DC/AO
 * PB3   ------> CSN/CE
 * PF2   ------> MOSI
 * 
 */
#include <string.h>
#include <stdio.h>
#include "main.h"
#include "py32f0xx_bsp_clock.h"
#include "py32f0xx_msp.h"
#include "SEGGER_RTT.h"
#include "st7567.h"
#include "xl2400.h"
#include "util.h"

// 0:RX, 1:TX, 2:TX_FAST
#define XL2400_MODE 0

const uint8_t TX_ADDRESS[5] = {0x11,0x33,0x33,0x33,0x11};
const uint8_t RX_ADDRESS[5] = {0x33,0x55,0x33,0x44,0x33};
uint8_t tmp[] = {
    0x1F, 0x80, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
    0x21, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x28,
    0x31, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x38,
    0x41, 0x12, 0x13, 0x14, 0x15, 0x16, 0x37, 0x48};
extern uint8_t xbuf[XL2400_PL_WIDTH_MAX + 1];

int main(void)
{
  int y1, y2;
  uint8_t d1, d2;
  uint32_t loop;

  BSP_RCC_HSI_PLL48MConfig();

  SEGGER_RTT_printf(0, "SPI Demo: ST7567 LCD\r\nClock: %ld\r\n", SystemCoreClock);

  /** 
   * Important: 
   * delay 2 seconds before SWD port stop working, so you will have 
   * enougth time to re-flash the MCU
  */
  LL_mDelay(2000);

  /* Check if PF0/PF2 pin has been set as GPIO pin*/
  if(READ_BIT(FLASH->OPTR, FLASH_OPTR_NRST_MODE) == OB_RESET_MODE_RESET)
  {
    SEGGER_RTT_WriteString(0, "Write option bytes\r\n");
    /* If not, turn off the RESET function on pin(PF0/PF2), this will reset the MCU */
    MSP_FlashSetOptionBytes();
  }
  SEGGER_RTT_WriteString(0, "Option bytes have been set\r\n");

  MSP_GPIO_Init();
  MSP_SPI_Init();

  while (XL2400_SPI_Test() == ERROR)
  {
    SEGGER_RTT_WriteString(0, " - check failed\r\n");
    LL_mDelay(1000);
  }
  SEGGER_RTT_WriteString(0, " - check passed\r\n");

  XL2400_Init();
  XL2400_SetPower(XL2400_RF_0DB);

  ST7567_Init();

  ST7567_Fill(0);
  LL_mDelay(2000);

  ST7567_DrawLine(0,   0, 127,  0, 1);
  ST7567_DrawLine(0,   0,   0, 63, 1);
  ST7567_DrawLine(127, 0, 127, 63, 1);
  ST7567_DrawLine(0,  63, 127, 63, 1);

  ST7567_GotoXY(5, 5);
  ST7567_Puts("LCD:ST7567", &Font_12x24, 1);
  ST7567_GotoXY(10, 42);
  ST7567_Puts("Font size: 12x24", &Font_6x8, 1);
  ST7567_UpdateScreen(); // display
  LL_mDelay(2000);

  /* Infinite loop */
  y1 = 0, y2 = 0, d1 = 0, d2 = 0;
  while(1)
  {
    if (XL2400_SPI_Test() == ERROR)
    {
      SEGGER_RTT_WriteString(0, " - check failed\r\n");
    }
    SEGGER_RTT_WriteString(0, " - check passed\r\n");
    LL_GPIO_TogglePin(GPIOB, LL_GPIO_PIN_6);
    for (loop = 0; loop < 0x08; loop++)
    {
      ST7567_GotoXY(y1+10, y2+17);
      ST7567_Puts("ST7567", &Font_6x8, 1);
      ST7567_GotoXY(y1+20, y2+33);
      ST7567_Puts("IOsetting", &Font_6x8, 1);
      ST7567_UpdateScreen();
      ST7567_Fill(0);
    }
    LL_mDelay(100);
    if (d1 == 0)
    {
      y1++;
      if (y1 == 54)
      {
        d1 = 1;
        if (d2 == 0)
        {
          y2 += 4;
          if (y2 == 20) d2 = 1;
        }
        else
        {
          y2 -= 4;
          if (y2 == 0) d2 = 0;
        }
      }
    }
    else
    {
      y1--;
      if (y1 == 0) d1 = 0;
    }
  }
}

uint8_t SPI_TxRxByte(uint8_t data)
{
  uint8_t SPITimeout = 0xFF;
  /* Check the status of Transmit buffer Empty flag */
  while (READ_BIT(SPI2->SR, SPI_SR_TXE) == RESET)
  {
    if (SPITimeout-- == 0) return 0;
  }
  LL_SPI_TransmitData8(SPI2, data);
  SPITimeout = 0xFF;
  while (READ_BIT(SPI2->SR, SPI_SR_RXNE) == RESET)
  {
    if (SPITimeout-- == 0) return 0;
  }
  // Read from RX buffer
  return LL_SPI_ReceiveData8(SPI2);
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
