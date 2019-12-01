/**
  ******************************************************************************
  * @file    w25x16.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
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
#ifndef __W25X16_H
#define __W25X16_H
#include "ebox_core.h"
#include "ebox_block.h"

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
////////////////////////////////////////////////////////////////////////////
//W25X16
#define FLASH_ID 0XEF14
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

typedef struct 
{
    uint16_t page_size;
    uint16_t page_count;
    uint16_t sector_size;
}W25xInfo_t;



class W25x16 : public Block
{
public:
    W25x16(Gpio *cs, Spi *spi)
    {
        this->cs  = cs;
        this->spi = spi;
    }
    int begin();
    virtual int init();
    
    uint16_t    read_id();
    
    virtual int read_sector(uint8_t *buffer, uint32_t sector, uint8_t count);
    void    read(uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
    void fast_read(uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
    
    void write(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
    virtual int write_sector(uint8_t *data, uint32_t sector, uint8_t count);

    virtual int erase_sector(uint32_t dst_addr);
    void erase_chip(void);

    
    uint8_t     read_sr(uint8_t index = 0);
    void        write_sr(uint8_t index,uint8_t value);
    void        wait_busy(void);
    void        power_down(void);
    void        wake_up(void);
    void        write_enable(void);
    void        write_disable(void);
    uint16_t    get_type();
    
private:
    uint8_t     spi_flash_buf[4096];
    Spi::Config_t cfg;
    Gpio        *cs;
    Spi         *spi;
    uint8_t     initialized;

    void        write_no_check(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
    void        write_page(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);

    uint16_t    type;				//定义W25QXX芯片型号		   


};
#endif
