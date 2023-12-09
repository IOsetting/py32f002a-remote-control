#ifndef __DRV_HSPWM_H
#define __DRV_HSPWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void DRV_HSPWM_Init(void);
void DRV_HSPWM_SetDuty(uint8_t ch, uint32_t duty);
/**
 * step can be negative for decrement
*/
void DRV_HSPWM_IncreaseDuty(uint8_t ch, int8_t step);


#ifdef __cplusplus
}
#endif

#endif /* __DRV_HSPWM_H */
