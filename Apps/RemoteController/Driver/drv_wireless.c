#include "drv_wireless.h"
#include "xl2400.h"

void DRV_Wireless_Init(uint8_t channel, uint8_t *rx_addr, uint8_t *tx_addr)
{
  XL2400_Init();
  XL2400_SetPower(XL2400_RF_0DB);
  XL2400_SetChannel(channel);
  XL2400_SetTxAddress(rx_addr);
  XL2400_SetRxAddress(tx_addr);
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