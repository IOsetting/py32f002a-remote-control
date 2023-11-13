#ifndef __DRV_LSIO_H
#define __DRV_LSIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void DRV_LSIO_Init(void);
void DRV_LSIO_Tick(void);
void DRV_LSIO_SetDuty(uint8_t channel, uint8_t numerator, uint8_t denominator);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_LSIO_H */
