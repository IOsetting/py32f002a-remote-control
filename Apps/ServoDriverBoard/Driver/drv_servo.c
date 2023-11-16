#include "drv_servo.h"

/**
 * Convert 3-channel analog input to 8-channel PWM duties
 * 
 *           |  1 -1 -1  1 |
 * (X Y Z) * |  1  1  1  1 | 
 *           |  1  1 -1 -1 |
 * 
*/
void DRV_SERVO_AnalogConvert(uint8_t uvx, uint8_t uvy, uint8_t uvz, uint8_t *direction, uint8_t *pwm_channel)
{
  uint8_t i;
  uint8_t *a, *b;
  // Turn unsigned analog values to signed values
  int16_t denominator = 0, vx = uvx - 127, vy = uvy - 127, vz = uvz - 127, motor[4];
  // Ignore the center point difference
  vx = (vx < 8 && vx > -7)? 0 : vx;
  vy = (vy < 8 && vy > -7)? 0 : vy;
  vz = (vz < 8 && vz > -7)? 0 : vz;
  // Convert to motor vectors
  motor[0] = vx + vy + vz;
  motor[1] = -vx + vy + vz;
  motor[2] = -vx + vy - vz;
  motor[3] = vx + vy - vz;

  // Use max value as denominator
  for (i = 0; i < 4; i++)
  {
    if (motor[i] > denominator) denominator = motor[i];
    else if (-motor[i] > denominator) denominator = -motor[0];
  }
  SEGGER_RTT_printf(0, "XYZ: %4d %4d %4d -> MOTOR: %4d %4d %4d %4d  DN: %4d\r\n", 
        vx, vy, vz, *motor, *(motor + 1), *(motor + 2), *(motor + 3), denominator);

  // Convert to PWM channels
  for (i = 0; i < 4; i++)
  {
    a = pwm_channel + (i * 2);
    b = a + 1;
    *a = 0;
    *b = 0;
    if (motor[i] > 0)
    {
      *a = (denominator > 255)? (uint8_t)(motor[i] * 255 / denominator) : (uint8_t)motor[i];
    }
    else
    {
      *b = (denominator > 255)? (uint8_t)((-motor[i]) * 255 / denominator): (uint8_t)(-motor[i]);
    }

    if (*(direction +i) == 1)
    {
      // reuse vx for swap
      vx = *a;
      *a= *b;
      *b = vx;
    }
  }
  SEGGER_RTT_printf(0, "pwm: %02X %02X, %02X %02X, %02X %02X, %02X %02X\r\n", 
        *pwm_channel, *(pwm_channel + 1), *(pwm_channel + 2), *(pwm_channel + 3), 
        *(pwm_channel + 4), *(pwm_channel + 5), *(pwm_channel + 6), *(pwm_channel + 7));

}