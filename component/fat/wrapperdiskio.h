#ifndef __VIRTUALDISKIO_H
#define __VIRTUALDISKIO_H
#include "stm32f10x.h"
#include "diskio.h"

#ifdef __cplusplus
extern "C" {
#endif

DRESULT attach_sd_to_fat(uint8_t pdrv, void *handler);
DSTATUS my_disk_status(uint8_t pdrv );
DSTATUS my_disk_initialize(uint8_t pdrv);
DRESULT my_disk_read(uint8_t pdrv, uint8_t *buffer, uint32_t sector, uint8_t count);
DRESULT my_disk_write(uint8_t pdrv, const uint8_t *data, uint32_t sector,  uint8_t count);
DRESULT my_disk_ioctl (uint8_t pdrv, uint8_t cmd,   void *buff);
    
#ifdef __cplusplus
}
#endif

#endif
