#ifndef __VIRTUALDISKIO_H
#define __VIRTUALDISKIO_H
#include "stm32f10x.h"

#ifdef __cplusplus
 extern "C" {
#endif
u8 SD_Init(void);
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count);
u8 SD_WriteMultiBlock(u32 sector,  const u8 *data, u8 count);

#ifdef __cplusplus
 }
#endif
 
#endif
