#ifndef __VIRTUALDISKIO_H
#define __VIRTUALDISKIO_H
#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif
     
void attach_sd_to_fat(void* sd);
     
     
int	MMC_disk_status(void);
uint8_t MMC_disk_initialize(void);
uint8_t MMC_disk_read(uint8_t *buffer,uint32_t sector, uint8_t count);
uint8_t MMC_disk_write( const uint8_t *data,uint32_t sector,  uint8_t count);
     
#ifdef __cplusplus
 }
#endif
 
#endif
