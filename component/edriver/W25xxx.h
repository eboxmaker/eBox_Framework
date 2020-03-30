/**
  ******************************************************************************
  * @file    w25xxx.h
  * @author  shentq
  * @version V2.2
  * @date    2019/08/14
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __W25XXX_H
#define __W25XXX_H

#include "ebox_core.h"
#include "interface/storage/blockdevice/BlockDevice.h"

using namespace ebox;
//0XEF13,表示芯片型号为W25Q80  
//0XEF14,表示芯片型号为W25Q16    
//0XEF15,表示芯片型号为W25Q32  
//0XEF16,表示芯片型号为W25Q64 
//0XEF17,表示芯片型号为W25Q128 	  
//0XEF18,表示芯片型号为W25Q256
#define W25Q80 	0XEF13 	
#define W25Q16 	0XEF14
#define W25Q32 	0XEF15
#define W25Q64 	0XEF16
#define W25Q128	0XEF17
#define W25Q256 0XEF18

#define W25X_PAGE_SIZE 256
#define W25X_SECTOR_SIZE 4096
////////////////////////////////////////////////////////////////////////////

//cmd
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg1		0x05 
#define W25X_ReadStatusReg2		0x35 
#define W25X_ReadStatusReg3		0x15 
#define W25X_WriteStatusReg1    0x01 
#define W25X_WriteStatusReg2    0x31 
#define W25X_WriteStatusReg3    0x11 
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
#define W25X_Enable4ByteAddr    0xB7
#define W25X_Exit4ByteAddr      0xE9


class W25xxx : public ebox::BlockDevice
{
public:
    W25xxx(Gpio *cs, Spi *spi)
    {
        this->cs  = cs;
        this->spi = spi;
    }
    virtual int begin();
    virtual int init();
    virtual int deinit();

    virtual int read(void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int program(const void *buffer, bd_addr_t addr, bd_size_t size);
    virtual int erase(bd_addr_t addr, bd_size_t size);
    virtual bd_size_t get_read_size() const;
    virtual bd_size_t get_program_size() const;
    virtual bd_size_t get_erase_size() const;
    virtual bd_size_t size() const ;
    virtual const char *get_type() const;
    virtual int get_erase_value() const;
    virtual bd_size_t get_erase_size(bd_addr_t addr) const;

    
    virtual int read_sector(uint8_t *buffer, uint32_t sector, uint8_t count);
    virtual int write_sector(const uint8_t *buffer, uint32_t sector, uint8_t count);
    
    
    
    void        read(uint8_t *buffer, uint32_t read_addr, uint16_t num_to_read);
    void        write(uint8_t *buffer, uint32_t write_addr, uint16_t num_to_write);
    virtual int erase_sector(uint32_t dst_addr);
    
    
    
    void        fast_read(uint8_t *buffer, uint32_t read_addr, uint16_t num_to_read);
    void        write_no_check(const uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
    void        erase_chip(void);

    uint8_t     read_sr(uint8_t index = 0);
    void        write_sr(uint8_t index,uint8_t value);
    void        wait_busy(void);
    void        power_down(void);
    void        wake_up(void);
    void        write_enable(void);
    void        write_disable(void);
    uint16_t    read_id();

private:
    Spi::Config_t cfg;
    Gpio        *cs;
    Spi         *spi;
    bool        initialized;

    void        write_page(const uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);

    DataU16_t   type;				//定义W25QXX芯片型号	
    uint32_t    page_count;

    bd_size_t capacity ;
    bd_size_t sector_size;
    bd_size_t sector_count;


};
//class W25xxx : public ebox::BlockDevice
//{
//public:
//    W25xxx(Gpio *cs, Spi *spi)
//    {
//        this->cs  = cs;
//        this->spi = spi;
//    }
//    virtual int begin();
//    virtual int read_sector(uint8_t *buffer, uint32_t sector, uint8_t count);
//    virtual int write_sector(const uint8_t *buffer, uint32_t sector, uint8_t count);
//    
//    
//    
//    void        read(uint8_t *buffer, uint32_t read_addr, uint16_t num_to_read);
//    void        fast_read(uint8_t *buffer, uint32_t read_addr, uint16_t num_to_read);
//    void        write(uint8_t *buffer, uint32_t write_addr, uint16_t num_to_write);
//    void        write_no_check(const uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
//    virtual int erase_sector(uint32_t dst_addr);
//    void        erase_chip(void);

//    uint8_t     read_sr(uint8_t index = 0);
//    void        write_sr(uint8_t index,uint8_t value);
//    void        wait_busy(void);
//    void        power_down(void);
//    void        wake_up(void);
//    void        write_enable(void);
//    void        write_disable(void);
//    uint16_t    get_type();
//    
//private:
//    uint8_t     spi_flash_buf[4096];
//    Spi::Config_t cfg;
//    Gpio        *cs;
//    Spi         *spi;
//    bool     initialized;

//    int         init();
//    uint16_t    read_id();
//    void        write_page(const uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);

//    uint16_t    type;				//定义W25QXX芯片型号	
//    uint32_t    page_count;


//};
#endif
