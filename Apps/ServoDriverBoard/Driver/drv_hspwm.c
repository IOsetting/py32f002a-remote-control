/***************
 * High Speed PWM
 * *************
*/

#include "drv_hspwm.h"
#include "py32f0xx_msp.h"


static uint8_t pwm_duty[HSPWM_CH_SIZE];

/**
 * Change this method to change the order of channels
*/
__STATIC_INLINE void updateChannel(uint8_t ch)
{
  switch (ch)
  {
  case 1:
    LL_TIM_OC_SetCompareCH3(TIM1, pwm_duty[ch -1]); // PA0
    break;

  case 2:
    LL_TIM_OC_SetCompareCH4(TIM1, pwm_duty[ch -1]); // PA1
    break;

  case 3:
    LL_TIM_OC_SetCompareCH1(TIM1, pwm_duty[ch -1]); // PA3
    break;

  case 4:
    LL_TIM_OC_SetCompareCH1(TIM3, pwm_duty[ch -1]); // PA6
    break;

  case 5:
    LL_TIM_OC_SetCompareCH2(TIM3, pwm_duty[ch -1]); // PA7
    break;

  case 6:
    LL_TIM_OC_SetCompareCH3(TIM3, pwm_duty[ch -1]); // PB0
    break;

  case 7:
    LL_TIM_OC_SetCompareCH4(TIM3, pwm_duty[ch -1]); // PB1
    break;

  case 8:
    LL_TIM_OC_SetCompareCH2(TIM1, pwm_duty[ch -1]); // PB3
    break;

  default:
    break;
  }
}

void DRV_HSPWM_SetDuty(uint8_t ch, uint32_t duty)
{
  pwm_duty[ch - 1] = duty;
#if DEBUG == 1
  SEGGER_RTT_printf(0, "%02d: %03d \r\n", ch, pwm_duty[ch - 1]);
#endif
  updateChannel(ch);
}

__STATIC_INLINE uint8_t increase(uint8_t val, int8_t step, uint8_t limit)
{
  if (step > 0)
  {
    val = val + step;
    if (val > limit) val = limit;
  }
  else if (step < 0)
  {
    step = -step;
    if (val >= step) val = val - step;
    else val = 0;
  }
  return val;
}

void DRV_HSPWM_IncreaseDuty(uint8_t ch, int8_t step)
{
  pwm_duty[ch - 1] = increase(pwm_duty[ch - 1], step, HSPWM_PERIOD);
#if DEBUG == 1
  SEGGER_RTT_printf(0, "%02d: %03d \r\n", ch, pwm_duty[ch - 1]);
#endif
  updateChannel(ch);
}

void DRV_HSPWM_Init(void)
{
  uint8_t i;
  for (i = 0; i < HSPWM_CH_SIZE; i++)
  {
    pwm_duty[i] = 0;
    DRV_HSPWM_SetDuty(i + 1, 0);
  }
}
