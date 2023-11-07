#ifndef __BSP_BASE_H
#define __BSP_BASE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

uint8_t SPI_TxRxByte(uint8_t data);

int itoa(int value, char *sp, int radix);

#ifdef __cplusplus
}
#endif

#endif /* __BSP_BASE_H */
