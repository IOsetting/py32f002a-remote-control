#include "drv_display.h"
#include "st7567.h"


static uint8_t y1 = 0, d1 = 0;

void BSP_Display_Init(void)
{
  uint8_t data[8] = {0x81, 0x81,0x42, 0x42, 0x24, 0x24, 0x18, 0x18};
  ST7567_Init();

  ST7567_FillAll(0);
  ST7567_WritePage(1, 10, data, 8);
  ST7567_WritePage(2, 20, data, 8);

  DRV_PutChar(3, 0, '0', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(3, 6, '1', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(3,12, '2', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(3,18, '3', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(4, 0, 'B', &Font_5x7, 5, 0, 1, 1);
  DRV_PutChar(4, 5, 'T', &Font_5x7, 5, 0, 1, 1);
  DRV_PutChar(4,10, 'B', &Font_5x7, 5, 0, 1, 1);
  DRV_PutChar(4,15, 'A', &Font_5x7, 5, 0, 1, 1);
  DRV_PutChar(4,20, 'B', &Font_5x7, 5, 0, 1, 1);
  DRV_PutChar(5, 0, '0', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(5, 6, '1', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(5,12, '2', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(5,18, '3', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(6, 0, 'B', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(6, 6, 'T', &Font_5x7, 6, 1, 1, 0);
  DRV_PutChar(6,12, 'E', &Font_5x7, 6, 1, 1, 1);
  DRV_PutChar(6,18, 'D', &Font_5x7, 6, 1, 1, 0);
  ST7567_WritePage(6, 24, data, 8);
  DRV_PutChar(7, 0, 'B', &Font_5x7, 6, 1, 1, 0);
  DRV_PutChar(7, 6, 'T', &Font_5x7, 6, 1, 1, 1);

  DRV_PutChar(7,12, 'A', &Font_5x7, 6, 1, 1, 0);
  DRV_PutChar(7,18, 'F', &Font_5x7, 6, 1, 1, 1);
  ST7567_WritePage(7, 24, data, 8);
}

void BSP_Display_Init2(void)
{
  ST7567_FillAll(0x88);
}

void BSP_Display_Init3(void)
{
  uint8_t i = 1, j = 8;

  while (j--)
  {
    ST7567_FillAll(i <<= 1);
    LL_mDelay(1000);
  }
}

void BSP_Display_Loop(void)
{
  ST7567_FillPage(y1++, 0, d1++, ST7567_WIDTH);
  y1 = y1 % 8;
}


void DRV_PutChar(uint8_t page, uint8_t column, char ch, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert)
{
  uint8_t tmp, data[charWidth], i, j;
  for (i = 0; i < charWidth; i++)
  {
    data[i] = colorInvert? 0xFF : 0x00;
    if (i < xOffset) continue;

    for (j = 0; j < font->height; j++)
    {
      tmp = font->data[(ch -32) * font->height + j];
      if ((tmp >> (i - xOffset)) & 1)
      {
        if (colorInvert)
        {
          data[i] = data[i] & ~(1 << (j + yOffset));
        }
        else
        {
          data[i] = data[i] | (1 << (j + yOffset));
        }
      }
      else
      {
        if (colorInvert)
        {
          data[i] = data[i] | (1 << (j + yOffset));
        }
        else
        {
          data[i] = data[i] & ~(1 << (j + yOffset));
        }
      }
      
    }
  }
  ST7567_WritePage(page, column, data, charWidth);
}
