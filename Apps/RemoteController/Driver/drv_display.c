#include "drv_display.h"
#include "st7567.h"

const uint8_t key_map[8] = {7, 6, 4, 5, 1, 0, 2, 3};
const uint8_t open[6] = {0xFF, 0x01, 0x01, 0x01, 0x01, 0xFF};
const uint8_t body[6] = {0xFF, 0x00, 0x00, 0x00, 0x00, 0xFF};
const uint8_t close[6] = {0xFF, 0x80, 0x80, 0x80, 0x80, 0xFF};

static void DRV_ui8toa(uint8_t value, char *sp, uint8_t width);
static void DRV_DrawChar(char ch, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert);
static void DRV_DrawRepeat(uint8_t symbol, uint8_t width, uint8_t offset, uint8_t colorInvert);
static void DRV_DrawBytes(uint8_t *pData, uint8_t size, int8_t offset, uint8_t colorInvert);
static void DRV_DrawHorizBar(uint8_t page, uint8_t column, uint8_t size);
static void DRV_DrawVertiBar(uint8_t column, uint8_t offset, uint8_t size);
static void DRV_DrawKeyState(uint8_t key_state);
static void DRV_DrawHorizBarCursor(uint8_t page, uint8_t column, uint8_t value, uint8_t barWidth, uint8_t cursorWidth, uint8_t direction);
static void DRV_DrawVertiBarCursor(uint8_t column, uint8_t offset, uint8_t size, uint8_t value, uint8_t barHeight, uint8_t direction);
static void DRV_DrawNumber(uint8_t page, uint8_t column, uint8_t num);

void DRV_Display_Reset(void)
{
  ST7567_Init();
  ST7567_FillAll(0);
}

void DRV_Display_Init(void)
{
  DRV_DrawHorizBar(0, 10, 50); // Horizontal Bar1
  DRV_DrawHorizBar(0, 65, 50); // Horizontal Bar2

  DRV_DrawHorizBar(7, 0, 60);  // Horizontal Bar3
  DRV_DrawHorizBar(7, 65, 60); // Horizontal Bar4

  DRV_DrawVertiBar(0, 1, 52);  // Vertical Bar1
  DRV_DrawVertiBar(121, 1, 52);// Vertical Bar2

  DRV_DrawHorizBar(6, 8, 10);  // Key bars
  DRV_DrawHorizBar(6, 22, 10);
  DRV_DrawHorizBar(6, 36, 10);
  DRV_DrawHorizBar(6, 50, 10);

  DRV_DrawHorizBar(6, 65, 10);
  DRV_DrawHorizBar(6, 79, 10);
  DRV_DrawHorizBar(6, 93, 10);
  DRV_DrawHorizBar(6, 107, 10);

  // DRV_PutString(4, 10, "CHECK THIS    ", &Font_5x7, 6, 0, 1, 0);
  // DRV_PutString(5, 10, "HIGH ALTITUDE ", &Font_5x7, 6, 0, 1, 0);
}

void DRV_Display_Update(uint8_t *state)
{
  DRV_DrawKeyState(*(state + 6));
  DRV_DrawHorizBarCursor(0, 10, *(state + 4), 50, 4, 0);
  DRV_DrawHorizBarCursor(0, 65, *(state + 5), 50, 4, 1);
  DRV_DrawHorizBarCursor(7,  0, *(state + 1), 60, 4, 0);
  DRV_DrawHorizBarCursor(7, 65, *(state + 2), 60, 4, 1);

  DRV_DrawVertiBar(0, 1, 52);
  DRV_DrawVertiBarCursor(0, 1, 52, *(state + 0), 4, 0);
  DRV_DrawVertiBar(121, 1, 52);
  DRV_DrawVertiBarCursor(121, 1, 52, *(state + 3), 4, 1);

  DRV_DrawNumber(1, 10, *(state + 4));
  DRV_DrawNumber(1, 100, *(state + 5));

  DRV_DrawNumber(4, 10, *(state + 0));
  DRV_DrawNumber(4, 100, *(state + 3));

  DRV_DrawNumber(5, 10, *(state + 1));
  DRV_DrawNumber(5, 100, *(state + 2));
}

static void DRV_DrawNumber(uint8_t page, uint8_t column, uint8_t num)
{
  char str[3];
  DRV_ui8toa(num, str, 3);
  DRV_PutChars(page, column, str, 3, &Font_5x7, 6, 0, 1, 0);
}

static void DRV_DrawHorizBarCursor(uint8_t page, uint8_t column, uint8_t value, uint8_t barWidth, uint8_t cursorWidth, uint8_t direction)
{
  value = direction? value : 255 - value;
  ST7567_SetCursor(page, column + 1);
  DRV_DrawRepeat(0x42, barWidth, 0, 0);
  ST7567_SetCursor(page, column + 1 + (value * (barWidth - cursorWidth) / 255));
  DRV_DrawRepeat(0x7E, cursorWidth, 0, 0);
}

static void DRV_DrawVertiBarCursor(uint8_t column, uint8_t offset, uint8_t size, uint8_t value, uint8_t barHeight, uint8_t direction)
{
  uint8_t data, page, pos;
  // page & offset of top border and bottom border
  uint8_t tp = offset / 8, to = offset % 8, bp = (offset + size + 1) / 8, bo = (offset + size + 1) % 8;

  value = direction? value : 255 - value;
  pos = value * (size - barHeight) / 255;
  pos = offset + 1 + pos;
  page = pos / 8;
  ST7567_SetCursor(page, column + 1);
  pos = pos % 8;
  while(8 - pos <= barHeight)
  {
    data = 0xFF << pos;
    if (page == tp)
    {
      data = data | (0x01 << to);
    }
    DRV_DrawRepeat(data, 4, 0, 0);
    barHeight = barHeight + pos - 8;
    pos = 0;
    ST7567_SetCursor(++page, column + 1);
  }
  // Ending
  data = 0xFF >> (8 - barHeight);
  data = data << pos;
  if (page == tp)
  {
    data = data | (0x01 << to);
  }
  if (page == bp)
  {
    data = data | (0x01 << bo);
  }
  DRV_DrawRepeat(data, 4, 0, 0);
}

static void DRV_DrawKeyState(uint8_t key_state)
{
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    ST7567_SetCursor(6, 9 + (14 * key_map[i]) + (key_map[i] > 3? 1 : 0));
    if ((key_state >> i) & 0x01)
    {
      DRV_DrawRepeat(0x42, 10, 0, 0);
    }
    else
    {
      DRV_DrawRepeat(0x7E, 10, 0, 0);
    }
  }
}

static void DRV_DrawHorizBar(uint8_t page, uint8_t column, uint8_t size)
{
  ST7567_SetCursor(page, column);
  DRV_DrawRepeat(0x7E, 1, 0, 0);
  DRV_DrawRepeat(0x42, size, 0, 0);
  DRV_DrawRepeat(0x7E, 1, 0, 0);
}

static void DRV_DrawVertiBar(uint8_t column, uint8_t offset, uint8_t size)
{
  uint8_t page;
  page = offset / 8;
  ST7567_SetCursor(page, column);

  offset = offset % 8;
  // Open
  DRV_DrawBytes((uint8_t *)open, 6, offset, 0);
  size = size - (8 - offset - 1);
  // Body
  while (size > 7)
  {
    ST7567_SetCursor(++page, column);
    DRV_DrawBytes((uint8_t *)body, 6, 0, 0);
    size = size - 8;
  }
  // Close
  ST7567_SetCursor(++page, column);
  DRV_DrawBytes((uint8_t *)close, 6, size - 7, 0);
}


/**
 * Print one ASCII character at specified page and column
 * charWdith: width in pixes
 * xOffset: offset in x direction, start from left-top
 * yOffset: offset in y direction, start from left-top
 * colorInvert: 0:no, 1:yes
*/
void DRV_PutChar(uint8_t page, uint8_t column, char ch, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert)
{
  ST7567_SetCursor(page, column);
  DRV_DrawChar(ch, font, charWidth, xOffset, yOffset, colorInvert);
}

void DRV_PutChars(uint8_t page, uint8_t column, char *chars, uint8_t len, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert)
{
  ST7567_SetCursor(page, column);
  while (len--)
  {
    DRV_DrawChar(*(chars++), font, charWidth, xOffset, yOffset, colorInvert);
  }
}

void DRV_PutString(uint8_t page, uint8_t column, char *str, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert)
{
  uint8_t ch;

  ST7567_SetCursor(page, column);
  while ((ch = *(str++)))
  {
    DRV_DrawChar(ch, font, charWidth, xOffset, yOffset, colorInvert);
  }
}

static void DRV_DrawChar(char ch, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert)
{
  uint8_t data[charWidth], i;
  for (i = 0; i < charWidth; i++)
  {
    if (i < xOffset || i >= (font->width + xOffset))
    {
      data[i] = colorInvert? 0xFF : 0x00;
    }
    else
    {
      data[i] = font->data[(ch -32) * font->width + (i - xOffset)];
      data[i] = data[i] << yOffset;
      data[i] = colorInvert? ~data[i] : data[i];
    }
  }
  ST7567_TransmitBytes(data, charWidth);
}

static void DRV_DrawRepeat(uint8_t symbol, uint8_t width, uint8_t offset, uint8_t colorInvert)
{
  symbol = symbol << offset;
  symbol = colorInvert? ~symbol : symbol;
  ST7567_TransmitRepeat(symbol, width);
}

static void DRV_DrawBytes(uint8_t *pData, uint8_t size, int8_t offset, uint8_t colorInvert)
{
  uint8_t ch;
  while(size--)
  {
    ch = *(pData++);
    ch = (offset > 0)? (ch << offset) : (ch >> -offset);
    ch = colorInvert? ~ch : ch;
    ST7567_TransmitByte(ch);
  }
}

static void DRV_ui8toa(uint8_t value, char *sp, uint8_t width)
{
    char tmp[width];
    char *tp = tmp;
    int i = width;
    while(i--) *(tp++) = ' ';

    tp = tmp;
    while (value || tp == tmp)
    {
        i = value % 10;
        value /= 10;
        *tp++ = i + '0';
    }
    i = width;
    tp = tmp + width - 1;
    while(i--)
    {
      *sp++ = *tp--;
    }
}