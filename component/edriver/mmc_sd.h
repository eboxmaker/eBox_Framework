/*
file   : mmc_sd.h
*/

#ifndef __MMC_SD_H
#define __MMC_SD_H

#include "ebox_core.h"
#include "spi.h"
#include "interface/storage/blockdevice/BlockDevice.h"
namespace ebox{
    
class SD : public ebox::BlockDevice
{

public:
    SD(Gpio *cs, Spi *spi)
    {
        this->cs  = cs;
        this->spi = spi;
    }
    virtual int read_sector( uint8_t *buffer, uint32_t sector,uint8_t count);
    virtual int write_sector(const uint8_t *buffer,uint32_t sector,   uint8_t count);
    
    
    virtual int init();
    virtual int deinit();

    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int erase(bd_addr_t addr, bd_size_t size);
    
    virtual bd_size_t get_read_size() const;
    virtual bd_size_t get_program_size() const;
    virtual bd_size_t size() const ;
    virtual const char *get_type() const;
    
    
    int         get_CID(uint8_t *cid_data);
    int         get_CSD(uint8_t *csd_data);        

private:
    

    int _initialise_card();
        
    ebox::bd_size_t _sectors;
    ebox::bd_size_t _sd_sectors();
    
    bool _is_valid_trim(ebox::bd_addr_t addr, ebox::bd_size_t size);

    
    
    
    
    
    
    
    
    ebox::bd_size_t  _get_capacity(void);


private:
    uint8_t     _wait(void);
    uint8_t     _send_command(uint8_t cmd, uint32_t arg, uint8_t crc);
    uint8_t     _send_command_no_deassert(uint8_t cmd, uint32_t arg, uint8_t crc);
    int         _receive_data(uint8_t *data, uint16_t len, uint8_t release);
    uint8_t     read_single_block(uint64_t sector, uint8_t *buffer);
    uint8_t     write_single_block(uint64_t sector, const  uint8_t *data);
    uint8_t     read_bytes(unsigned long address, unsigned char *buf, unsigned int offset, unsigned int bytes);

public:
    uint8_t  SD_Type; //SD��������

private:
    Gpio            *cs;
    Spi             *spi;
    Spi::Config_t SPIDevSDCard;
    bool     initialized;


    static const uint32_t _block_size;
    uint32_t _erase_size;
    bool _is_initialized;
    bool _dbg;
    uint32_t _init_ref_count;

#if MBED_CONF_SD_CRC_ENABLED
    bool _crc_on;
#endif










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

}
#endif
