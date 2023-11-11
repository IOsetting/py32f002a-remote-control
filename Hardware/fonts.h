#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct {
    uint8_t width;
    uint8_t height;
    uint8_t order;
    uint8_t bytes;
    const uint8_t *data;
} FontDef_t;

extern FontDef_t Font_5x7;

#endif // __FONTS_H__
