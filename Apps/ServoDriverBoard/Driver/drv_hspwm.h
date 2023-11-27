#ifndef __DRV_HSPWM_H
#define __DRV_HSPWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void DRV_HSPWM_Init(void);
void DRV_HSPWM_SetDutyCH1(uint32_t duty);
void DRV_HSPWM_SetDutyCH2(uint32_t duty);
void DRV_HSPWM_SetDutyCH3(uint32_t duty);
void DRV_HSPWM_SetDutyCH4(uint32_t duty);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_HSPWM_H */
