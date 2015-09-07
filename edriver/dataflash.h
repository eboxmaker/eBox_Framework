#ifndef   __DATAFLASH_H__
#define   __DATAFLASH_H__

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
	int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) ;
	int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite);

	private:
		uint16_t Flash_Write_Without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite);

};


#ifdef __cplusplus
	}
#endif 
	
#endif
	