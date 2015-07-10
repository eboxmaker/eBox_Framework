/*
file   : w25x16.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __W25X16_H
#define __W25X16_H

#include "ebox.h"


////////////////////////////////////////////////////////////////////////////
//W25X16
#define FLASH_ID 0XEF14
//cmd
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 
class W25X:public SPIClass
{
	public:
		W25X(GPIO* cspin,SPI_TypeDef *spi,GPIO* sck,GPIO* miso,GPIO* mosi):SPIClass(SPI1,sck,miso,mosi)
		{
			cs = cspin;
		}
		void begin();
		void readId(uint16_t* id);
		void read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead); 
		void fastRead(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);
		void write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);

		void eraseSector(u32 Dst_Addr);
		void eraseChip(void);


	private:
		u8 SPI_FLASH_BUF[4096];
		SPICONFIG spiDevW25x16;

		GPIO* cs;
		u8 readSR(void);  
		void _waitBusy(void);
		void powerDown(void);	
		void wakeUp(void);
		void writeSR(u8 sr);
		void writeEnable(void);
		void writeDisable(void);
	
		void writePage(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
		void writeNoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);


};
#endif
