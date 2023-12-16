#ifndef __DRV_SYS_H
#define __DRV_SYS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"

/**
 * Read MCU UID
 * 
 * @param start  range [0, 127]
 * @param size   range [1, 128]
*/
void DRV_SYS_ReadDeviceUID(uint32_t *pBuf, uint8_t start, uint8_t size);

#ifdef __cplusplus
}
#endif

#endif /* __DRV_SYS_H */
