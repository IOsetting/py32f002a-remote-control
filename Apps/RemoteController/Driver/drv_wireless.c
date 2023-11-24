#include "drv_wireless.h"
#include "py32f0xx_msp.h"
#include "string.h"

#ifdef USE_XL2400

#include "xl2400.h"

void DRV_Wireless_Init(uint8_t channel, uint8_t *rx_addr, uint8_t *tx_addr)
{
  XL2400_Init();
  XL2400_SetPower(XL2400_RF_0DB);
  XL2400_SetChannel(channel);
  XL2400_SetRxAddress(rx_addr);
  XL2400_SetTxAddress(tx_addr);
  XL2400_SetTxMode();
}

uint8_t DRV_Wireless_Tx(uint8_t *data)
{
  return XL2400_Tx(data, XL2400_PLOAD_WIDTH);
}

ErrorStatus DRV_Wireless_Test(void)
{
  return XL2400_SPI_Test();
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