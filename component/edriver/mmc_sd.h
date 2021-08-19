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
    uint8_t  SD_Type; //SD卡的类型

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

}
#endif
