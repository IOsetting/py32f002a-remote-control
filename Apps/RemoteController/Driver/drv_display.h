#ifndef __BSP_DISPLAY_H
#define __BSP_DISPLAY_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "fonts.h"

void DRV_Display_Reset(void);
void DRV_Display_Init(void);
void DRV_Display_Update(uint8_t *state);

void DRV_PutChar(uint8_t page, uint8_t column, char ch, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert);
void DRV_PutChars(uint8_t page, uint8_t column, char *chars, uint8_t len, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert);
void DRV_PutString(uint8_t page, uint8_t column, char *ch, FontDef_t* font, uint8_t charWidth, uint8_t xOffset, uint8_t yOffset, uint8_t colorInvert);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_DISPLAY_H */
