#include "drv_display.h"
#include "st7567.h"

static uint8_t y1 = 0, d1 = 0;

void BSP_Display_Init(void)
{
  uint8_t i = 1, j = 8;
  ST7567_Init();
  while (j--)
  {
    ST7567_FillAll(i <<= 1);
    LL_mDelay(1000);
  }
}

void BSP_Display_Init2(void)
{
  ST7567_FillAll(0x88);
}

void BSP_Display_Init3(void)
{
  ST7567_FillAll(0);
}

void BSP_Display_Loop(void)
{
  ST7567_FillPage(y1++, 0, d1++, ST7567_WIDTH);
  y1 = y1 % 8;
}