#include "drv_wireless.h"
#include "py32f0xx_msp.h"
#include "string.h"

#ifdef USE_XL2400

#include "xl2400.h"

extern uint8_t xbuf[XL2400_PL_WIDTH_MAX + 1];

void DRV_Wireless_Init(uint8_t channel, uint8_t *rx_addr, uint8_t *tx_addr)
{
  XL2400_Init();
  XL2400_SetPower(XL2400_RF_6DB);
  XL2400_SetChannel(channel);
  XL2400_SetRxAddress(0, rx_addr);
  XL2400_SetTxAddress(tx_addr);
  XL2400_SetTxMode();
}

void DRV_Wireless_TxMode(uint8_t channel)
{
  XL2400_SetChannel(channel);
  XL2400_SetTxMode();
}

void DRV_Wireless_RxMode(uint8_t channel)
{
  XL2400_SetChannel(channel);
  XL2400_SetRxMode();
}

void DRV_Wireless_SetTxAddress(uint8_t *tx_addr)
{
  XL2400_SetTxAddress(tx_addr);
}

uint8_t DRV_Wireless_Tx(uint8_t *data)
{
  return XL2400_Tx(data, XL2400_PLOAD_WIDTH);
}

ErrorStatus DRV_Wireless_Test(void)
{
  return XL2400_SPI_Test();
}

ErrorStatus DRV_Wireless_Rx(uint8_t *pipe, uint8_t *data)
{
  uint8_t crc, i;

  i = XL2400_Rx();
  if (i & XL2400_FLAG_RX_DR)
  {
    *pipe = (i & 0x0E) >> 1;

    // CRC check
    crc = 0;
    for (i = 0; i < XL2400_PLOAD_WIDTH - 1; i++)
    {
      DEBUG_PRINTF("%02X ", *(xbuf + i));
      crc += *(xbuf + i);
    }
    DEBUG_PRINTF("%02X, CRC:%02X\r\n", *(xbuf + XL2400_PLOAD_WIDTH - 1), crc);
    if (crc != *(xbuf + XL2400_PLOAD_WIDTH - 1))
    {
      DEBUG_PRINT_STRING("CRC Error\r\n");
      return ERROR;
    }
    else
    {
      DEBUG_PRINT_STRING("CRC OK\r\n");
      // Store received data
      memcpy(data, xbuf, 7);
    }
    return SUCCESS;
  }
  else
  {
    return ERROR;
  }
}

#endif

#ifdef USE_XN297L

#include "xn297l.h"

void DRV_Wireless_Init(uint8_t channel, uint8_t *rx_addr, uint8_t *tx_addr)
{
  XN297L_Init();
  XN297L_SetChannel(channel);
  XN297L_SetRxAddress(rx_addr);
  XN297L_SetTxAddress(tx_addr);
  XN297L_SetTxMode();
}

uint8_t DRV_Wireless_Tx(uint8_t *data)
{
  return XN297L_TxData(data, XN297L_PLOAD_WIDTH);
}

ErrorStatus DRV_Wireless_Test(void)
{
  return XN297L_SPI_Test();
}

#endif