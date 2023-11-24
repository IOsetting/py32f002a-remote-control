#ifndef __DRV_WIRELESS_H
#define __DRV_WIRELESS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void DRV_Wireless_Init(uint8_t channel, uint8_t *rx_addr, uint8_t *tx_addr);
uint8_t DRV_Wireless_Tx(uint8_t *data);
ErrorStatus DRV_Wireless_Rx(uint8_t *data);
ErrorStatus DRV_Wireless_Test(void);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_WIRELESS_H */
