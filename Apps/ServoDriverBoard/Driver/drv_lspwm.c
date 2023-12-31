/***************
 * Low Speed PWM
 * *************
*/

#include "drv_lspwm.h"
#include "py32f0xx_msp.h"
#include "74hc595.h"
#include <string.h>


static uint8_t hc595_state[HC595_SIZE], pwm_duty[LSPWM_CH_SIZE], pwm_duty_pre[LSPWM_CH_SIZE], pwm_counter = 0;

void DRV_LSPWM_Init(void)
{
  uint8_t i;
  for (i = 0; i < LSPWM_CH_SIZE; i++)
  {
    pwm_duty[i] = 0;
    pwm_duty_pre[i] = 0;
  }
}

/**
 * channel: 0 ~ LSPWM_CH_SIZE - 1
 * numerator/denominator: duty
*/
void DRV_LSPWM_SetDuty(uint8_t channel, uint8_t numerator, uint8_t denominator)
{
  pwm_duty_pre[channel] = (uint8_t)(((uint16_t)numerator * LSPWM_PERIOD) / denominator);
}

void DRV_LSPWM_Tick(void)
{
  uint8_t i, hc595_idx, mask;

  for (i = 0; i < LSPWM_CH_SIZE; i++)
  {
    hc595_idx = i / 8;
    mask = 1 << (i % 8);

    if (pwm_duty[i] <= pwm_counter)
    {
      hc595_state[hc595_idx] = hc595_state[hc595_idx] & (~mask);
    }
    else
    {
      hc595_state[hc595_idx] = hc595_state[hc595_idx] | mask;
    }
  }
  /* Write to 74hc595 output pins */
  HC595_WriteBytes(hc595_state, HC595_SIZE);

  pwm_counter++;
  /* When period ends*/
  if (pwm_counter == LSPWM_PERIOD)
  {
    /* reset counter */
    pwm_counter = 0;
    /* reload comparison values */
    memcpy(pwm_duty, pwm_duty_pre, LSPWM_CH_SIZE);
  }
}