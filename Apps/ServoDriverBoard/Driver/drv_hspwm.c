/***************
 * High Speed PWM
 * *************
*/

#include "drv_hspwm.h"
#include "py32f0xx_msp.h"


static uint8_t pwm_duty[HSPWM_CH_SIZE];

void DRV_HSPWM_SetDutyCH1(uint32_t duty)
{
#if DEBUG == 1
  SEGGER_RTT_printf(0, "%03d \r\n", pwm_duty[0]);
#endif
  pwm_duty[0] = (pwm_duty[0] + 1) % 120;
  LL_TIM_OC_SetCompareCH3(TIM1, pwm_duty[0]);
}

void DRV_HSPWM_SetDutyCH2(uint32_t duty)
{
#if DEBUG == 1
  SEGGER_RTT_printf(0, "%03d \r\n", pwm_duty[1]);
#endif
  pwm_duty[1] = (pwm_duty[1] + 1) % 120;
  LL_TIM_OC_SetCompareCH4(TIM1, pwm_duty[1]);
}

void DRV_HSPWM_SetDutyCH3(uint32_t duty)
{
#if DEBUG == 1
  SEGGER_RTT_printf(0, "%03d \r\n", pwm_duty[2]);
#endif
  pwm_duty[2] = (pwm_duty[2] + 1) % 120;
  LL_TIM_OC_SetCompareCH1(TIM1, pwm_duty[2]);
}

void DRV_HSPWM_SetDutyCH4(uint32_t duty)
{
#if DEBUG == 1
  SEGGER_RTT_printf(0, "%03d \r\n", pwm_duty[3]);
#endif
  pwm_duty[3] = (pwm_duty[3] + 1) % 120;
  LL_TIM_OC_SetCompareCH2(TIM1, pwm_duty[3]);
}

void DRV_HSPWM_Init(void)
{
  uint8_t i;
  for (i = 0; i < HSPWM_CH_SIZE; i++)
  {
    pwm_duty[i] = 0;
  }
  DRV_HSPWM_SetDutyCH1(0);
  DRV_HSPWM_SetDutyCH2(0);
  DRV_HSPWM_SetDutyCH3(0);
  DRV_HSPWM_SetDutyCH4(0);
}
