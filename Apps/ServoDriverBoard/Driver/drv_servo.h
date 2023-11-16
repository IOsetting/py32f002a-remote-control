#ifndef __DRV_SERVO_H
#define __DRV_SERVO_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

void DRV_SERVO_AnalogConvert(uint8_t uvx, uint8_t uvy, uint8_t uvz, uint8_t *direction, uint8_t *pwm_channel);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_SERVO_H */
