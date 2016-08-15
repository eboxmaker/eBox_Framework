#ifndef __VIRTUALDISKIO_H
#define __VIRTUALDISKIO_H
#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif
     
void attach_sd_to_fat(void* sd);
     
     
int	MMC_disk_status(void);
u8 MMC_disk_initialize(void);
u8 MMC_disk_read(u8 *buffer,u32 sector, u8 count);
u8 MMC_disk_write( const u8 *data,u32 sector,  u8 count);
     
#ifdef __cplusplus
 }
#endif
 
#endif
