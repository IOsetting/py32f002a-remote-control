#include <string.h>
#include <stdio.h>
#include "main.h"
#include "py32f0xx_bsp_clock.h"
#include "py32f0xx_msp.h"
#include "drv_display.h"
#include "drv_wireless.h"
#include "74hc165.h"

typedef enum
{
  STATE_PAIR, STATE_RUN
} STATE_T;

STATE_T device_state;

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

const uint8_t channel = 78;
const uint8_t RX_ADDRESS[5] = {0x11,0x33,0x33,0x33,0x11};
uint8_t tx_addr[8] = {0x11,0x22,0x33,0x44,0x55}; //063431322B
//uint8_t tx_addr[8] = {0x06,0x34,0x31,0x32,0x2B}; //063431322B

void APP_Init(void);
STATE_T APP_Pair(void);
STATE_T APP_Run(void);

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

  APP_Init();

  device_state = STATE_RUN;
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
    LL_mDelay(10);
  }
}

void APP_Init(void)
{
  DRV_Display_Reset();
  DRV_Display_Init();

  while (DRV_Wireless_Test() == ERROR)
  {
    DEBUG_PRINT_STRING(" - check failed\r\n");
    LL_mDelay(1000);
  }
  DEBUG_PRINT_STRING(" - check passed\r\n");

  DRV_Wireless_Init(channel, (uint8_t *)RX_ADDRESS, (uint8_t *)tx_addr);

  wireless_state[10] = 0;
}

STATE_T APP_Pair(void)
{
  uint8_t i, j = 128;
  /* RX channel should be 1MHz lower than TX */
  DRV_Wireless_RxMode(channel - 2);

  DEBUG_PRINT_STRING(".");
  while(j--)
  {
    if (DRV_Wireless_Rx(&i, tx_addr) == SUCCESS)
    {
#if DEBUG == 1
      DEBUG_PRINT_STRING("Data received: ");
      for (i = 0; i < 8; i++)
      {
        DEBUG_PRINTF("%02X ", *(tx_addr + i));
      }
      DEBUG_PRINT_STRING("\r\n");
#endif
      DRV_Wireless_SetTxAddress(tx_addr);
      DRV_Wireless_TxMode(channel);
      DEBUG_PRINT_STRING("Enter RUN state\r\n");
      return STATE_RUN;
    }
    LL_mDelay(1);
  }
  return STATE_PAIR;
}

STATE_T APP_Run(void)
{
  static uint8_t pair_trigger_count = 0;

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
  if ((pad_state[6] & 0x48) == 0)
  {
    pair_trigger_count++;
    if (pair_trigger_count > 100)
    {
      pair_trigger_count = 0;
      DEBUG_PRINT_STRING("Enter PAIR state\r\n");
      return STATE_PAIR;
    }
  }
  return STATE_RUN;
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
