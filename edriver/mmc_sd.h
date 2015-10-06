/*
file   : mmc_sd.h
*/

#ifndef __MMC_SD_H
#define __MMC_SD_H

#include "ebox.h"

class SD
{

	public:
			SD(GPIO* cspin,SPI* p_SPI)
			{
				cs = cspin;
				spi = p_SPI;
			}
			int begin(uint8_t dev_num);
			uint8_t init();
			int get_CID(u8 *cid_data);
			int get_CSD(u8 *csd_data);
			u32 get_capacity(void);
			u8 read_single_block(u32 sector, u8 *buffer);
			u8 write_single_block(u32 sector,  u8 *data);
			u8 read_multi_block(u32 sector, u8 *buffer, u8 count);
			u8 write_multi_block(u32 sector,  const u8 *data, u8 count);
			u8 read_bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);


	private:
			uint8_t _wait(void);
			uint8_t _send_command(u8 cmd, u32 arg,u8 crc);
			uint8_t _send_command_no_deassert(u8 cmd, u32 arg,u8 crc);
			int _receive_data(u8 *data, u16 len, u8 release);


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
