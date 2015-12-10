/*
file   : iflash.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef   __IFLASH_H__
#define   __IFLASH_H__

#ifdef __cplusplus
 extern "C" {
#endif 
	 
#include "stm32f10x.h"
#include "stm32f10x_flash.h"




#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
  
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif


	 
class FLASHCLASS
{
    public:
        int read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) ;
        int write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite);
    private:
        uint16_t write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite);

};


#ifdef __cplusplus
	}
#endif

#endif
