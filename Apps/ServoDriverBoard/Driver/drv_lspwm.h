#ifndef __DRV_LSPWM_H
#define __DRV_LSPWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void DRV_LSPWM_Init(void);
void DRV_LSPWM_Tick(void);
void DRV_LSPWM_SetDuty(uint8_t channel, uint8_t numerator, uint8_t denominator);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_LSPWM_H */
