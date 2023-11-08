#include "drv_display.h"
#include "st7567.h"

static int y1 = 0, y2 = 0;
static uint8_t d1 = 0, d2 = 0;

void BSP_Display_Init(void)
{
  ST7567_Init();

  ST7567_Fill(0);
  ST7567_DrawLine(0,   0, 127,  0, 1);
  ST7567_DrawLine(0,   0,   0, 63, 1);
  ST7567_DrawLine(127, 0, 127, 63, 1);
  ST7567_DrawLine(0,  63, 127, 63, 1);

  ST7567_GotoXY(5, 5);
  ST7567_Puts("LCD:ST7567", &Font_12x24, 1);
  ST7567_GotoXY(10, 42);
  ST7567_Puts("Font size: 12x24", &Font_6x8, 1);
  ST7567_UpdateScreen(); // display
}

void BSP_Display_Loop(void)
{
  uint32_t loop;

  for (loop = 0; loop < 0x08; loop++)
  {
    ST7567_GotoXY(y1 + 10, y2 + 17);
    ST7567_Puts("ST7567", &Font_6x8, 1);
    ST7567_GotoXY(y1 + 20, y2 + 33);
    ST7567_Puts("IOsetting", &Font_6x8, 1);
    ST7567_UpdateScreen();
    ST7567_Fill(0);
  }
  LL_mDelay(100);
  if (d1 == 0)
  {
    y1++;
    if (y1 == 54)
    {
      d1 = 1;
      if (d2 == 0)
      {
        y2 += 4;
        if (y2 == 20) d2 = 1;
      }
      else
      {
        y2 -= 4;
        if (y2 == 0) d2 = 0;
      }
    }
  }
  else
  {
    y1--;
    if (y1 == 0) d1 = 0;
  }
}