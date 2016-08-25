/*
file   : mmc_sd.h
*/

#ifndef __MMC_SD_H
#define __MMC_SD_H

#include "ebox.h"

class SD
{

public:
    SD(Gpio *cs, Spi *spi)
    {
        this->cs  = cs;
        this->spi = spi;
    }
    int         begin(uint8_t dev_num);
    uint8_t     init();
    int         get_CID(uint8_t *cid_data);
    int         get_CSD(uint8_t *csd_data);
    uint64_t    get_capacity(void);
    uint8_t     read_single_block(uint32_t sector, uint8_t *buffer);
    uint8_t     write_single_block(uint32_t sector, const  uint8_t *data);
    uint8_t     read_multi_block(uint32_t sector, uint8_t *buffer, uint8_t count);
    uint8_t     write_multi_block(uint32_t sector,  const uint8_t *data, uint8_t count);
    uint8_t     read_bytes(unsigned long address, unsigned char *buf, unsigned int offset, unsigned int bytes);

private:
    uint8_t     _wait(void);
    uint8_t     _send_command(uint8_t cmd, uint32_t arg, uint8_t crc);
    uint8_t     _send_command_no_deassert(uint8_t cmd, uint32_t arg, uint8_t crc);
    int         _receive_data(uint8_t *data, u16 len, uint8_t release);

public:
    uint8_t  SD_Type; //SD��������

private:
    Gpio            *cs;
    Spi             *spi;
    SPI_CONFIG_TYPE SPIDevSDCard;


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
