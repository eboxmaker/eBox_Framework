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

class SD
{

	public:
			SD(GPIO* cspin,SPI *pSPI)
			{
				cs = cspin;
				spi = pSPI;
			}
			int begin(uint8_t dev_num);
			uint8_t init();
			int getCID(u8 *cid_data);
			int getCSD(u8 *csd_data);
			u32 getCapacity(void);
			u8 readSingleBlock(u32 sector, u8 *buffer);
			u8 writeSingleBlock(u32 sector,  u8 *data);
			u8 readMultiBlock(u32 sector, u8 *buffer, u8 count);
			u8 writeMultiBlock(u32 sector,  const u8 *data, u8 count);
			u8 readBytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);


	private:
			uint8_t _wait(void);
			uint8_t _sendCommand(u8 cmd, u32 arg,u8 crc);
			uint8_t _sendCommandNoDeassert(u8 cmd, u32 arg,u8 crc);
			int _receiveData(u8 *data, u16 len, u8 release);


	public:
		u8  SD_Type; //SD��������	 

	private:
		GPIO* cs;
		SPI_CONFIG_TYPE SPIDevSDCard;
		SPI* spi;
	
	
};

/* SD�����Ͷ��� */
#define SD_TYPE_MMC     0
#define SD_TYPE_V1      1
#define SD_TYPE_V2      2
#define SD_TYPE_V2HC    4


/* SD�������ݽ������Ƿ��ͷ����ߺ궨�� */
#define NO_RELEASE      0
#define RELEASE         1
							  
/* SD��ָ��� */
#define CMD0    0      //����λ  (Ӧ���ʽ��R1)
#define CMD1    1      //MMC����ʼ��ʼ��
#define CMD8	8     //ʶ�𿨵İ汾
#define CMD9    9     //����9 ����CSD����   (Ӧ���ʽ��R1)
#define CMD10   10     //����10����CID����   (Ӧ���ʽ��R1)
#define CMD12   12      //����12��ֹͣ���ݴ���    (Ӧ���ʽ��R1b)
#define CMD16   16      //����16������SectorSize Ӧ����0x00   (Ӧ���ʽ��R1)
#define CMD17   17      //����17����sector    (Ӧ���ʽ��R1)
#define CMD18   18      //����18����Multi sector    (Ӧ���ʽ��R1)
#define ACMD23  23      //����23�����ö�sectorд��ǰԤ�Ȳ���N��block    (Ӧ���ʽ��R1)
#define CMD24   24      //����24��дsector    (Ӧ���ʽ��R1)
#define CMD25   25     //����25��дMulti sector    (Ӧ���ʽ��R1)
#define ACMD41  41      //����41��Ӧ����0x00    (Ӧ���ʽ��R1)
#define CMD55   55      //����55��Ӧ����0x01    (Ӧ���ʽ��R1)
#define CMD58   58      //����58����OCR��Ϣ     (Ӧ���ʽ��R1)
#define CMD59   59      //����59��ʹ��/��ֹCRC��Ӧ����0x00    (Ӧ���ʽ��R1)

#endif
