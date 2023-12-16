#include "drv_sys.h"

void DRV_SYS_ReadDeviceUID(uint32_t *pBuf, uint8_t start, uint8_t size)
{
  while(size--)
  {
    *(pBuf++) = *((uint32_t *)UID_BASE_ADDRESS + (start++));
  }
}