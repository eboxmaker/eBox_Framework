/*
file   : mmc_sd.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __MMC_SD_H
#define __MMC_SD_H

#include "ebox.h"

class SD:public SPIClASS
{

	public:
			SD(GPIO* cspin,SPI_TypeDef *spi,GPIO* sck,GPIO* miso,GPIO* mosi):SPIClASS(spi,sck,miso,mosi)
			{
				cs = cspin;
			}
			int begin(void);
			
			uint8_t wait(void);
			uint8_t sendCommand(u8 cmd, u32 arg,u8 crc);
			uint8_t sendCommandNoDeassert(u8 cmd, u32 arg,u8 crc);
			uint8_t init();
			int receiveData(u8 *data, u16 len, u8 release);
			int getCID(u8 *cid_data);
			int getCSD(u8 *csd_data);
			u32 getCapacity(void);
			u8 readSingleBlock(u32 sector, u8 *buffer);
			u8 writeSingleBlock(u32 sector,  u8 *data);
			u8 readMultiBlock(u32 sector, u8 *buffer, u8 count);
			u8 writeMultiBlock(u32 sector,  const u8 *data, u8 count);
			u8 readBytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);



	public:
		u8  SD_Type; //SD卡的类型	 

	private:
		GPIO* cs;
		SPICONFIG SPIDevSDCard;
	
	
};

/* SD卡类型定义 */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4


/* SD传输数据结束后是否释放总线宏定义 */
#define NO_RELEASE      0
#define RELEASE         1
							  
/* SD卡指令表 */
#define CMD0    0      //卡复位  (应答格式：R1)
#define CMD1    1      //MMC卡开始初始化
#define CMD8	8     //识别卡的版本
#define CMD9    9     //命令9 ，读CSD数据   (应答格式：R1)
#define CMD10   10     //命令10，读CID数据   (应答格式：R1)
#define CMD12   12      //命令12，停止数据传输    (应答格式：R1b)
#define CMD16   16      //命令16，设置SectorSize 应返回0x00   (应答格式：R1)
#define CMD17   17      //命令17，读sector    (应答格式：R1)
#define CMD18   18      //命令18，读Multi sector    (应答格式：R1)
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block    (应答格式：R1)
#define CMD24   24      //命令24，写sector    (应答格式：R1)
#define CMD25   25     //命令25，写Multi sector    (应答格式：R1)
#define ACMD41  41      //命令41，应返回0x00    (应答格式：R1)
#define CMD55   55      //命令55，应返回0x01    (应答格式：R1)
#define CMD58   58      //命令58，读OCR信息     (应答格式：R1)
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00    (应答格式：R1)

#endif
