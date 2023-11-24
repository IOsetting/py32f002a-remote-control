#include <string.h>
#include <stdio.h>
#include "main.h"
#include "py32f0xx_bsp_clock.h"
#include "py32f0xx_msp.h"
#include "drv_display.h"
#include "drv_wireless.h"
#include "74hc165.h"

// 6-channel analog data
__IO uint16_t adc_dma_data[6];

/*
 * [0:5]: Analog channel 1 - 6
 * [6]:   Key 1 - 8
 * [7]:   CRC
*/ 
uint8_t pad_state[8];
/*
 * [0:4]: RX address
 * [5:9]: TX address
 * [10]:  success rate (0x00 - 0xFF)
 */
uint8_t wireless_state[11];
uint8_t wireless_tx = 0, wireless_tx_succ = 0;

const uint8_t TX_ADDRESS[5] = {0x11,0x33,0x33,0x33,0x11};
const uint8_t RX_ADDRESS[5] = {0x33,0x55,0x33,0x44,0x33};

int main(void)
{
  BSP_RCC_HSI_PLL48MConfig();

  DEBUG_PRINTF("Remote Control: Controller\r\nClock: %ld\r\n", SystemCoreClock);

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

  MSP_DMA_Config();
  MSP_ADC_Init();
  MSP_TIM1_Init();

  DRV_Display_Reset();
  DRV_Display_Init();

  while (DRV_Wireless_Test() == ERROR)
  {
    DEBUG_PRINT_STRING(" - check failed\r\n");
    LL_mDelay(1000);
  }
  DEBUG_PRINT_STRING(" - check passed\r\n");

  DRV_Wireless_Init(78, (uint8_t *)RX_ADDRESS, (uint8_t *)TX_ADDRESS);

  wireless_state[10] = 0;

  /* Infinite loop */
  while(1)
  {
#if DEBUG == 1
    for (uint8_t i = 0; i < 8; i++)
    {
      DEBUG_PRINTF("%02X ", pad_state[i]);
    }
    DEBUG_PRINT_STRING("\r\n");
#endif
    DRV_Display_Update(pad_state);
    // Send
    wireless_tx++;
    if (DRV_Wireless_Tx(pad_state) == 0x20)
    {
      wireless_tx_succ++;
    }
    if (wireless_tx == 0xFF)
    {
      wireless_state[10] = wireless_tx_succ;
      DEBUG_PRINTF("TX_SUCC: %02X\r\n", wireless_tx_succ);
      wireless_tx = 0;
      wireless_tx_succ = 0;
    }
    LL_mDelay(50);
  }
}

void DMA1_Channel1_IRQHandler(void)
{
  uint8_t crc = 0;
  if (LL_DMA_IsActiveFlag_TC1(DMA1) == 1)
  {
    LL_DMA_ClearFlag_TC1(DMA1);
    for (uint8_t i = 0; i < 6; i++)
    {
      pad_state[i] = (uint8_t)(*(adc_dma_data + i) >> 4);
      crc += pad_state[i];
    }
    // Read from 74HC165
    pad_state[6] = HC165_Read();
    pad_state[7] = crc + pad_state[6];
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
