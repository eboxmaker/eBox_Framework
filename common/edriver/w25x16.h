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
class W25x16
{
public:
    W25x16(Gpio *cs, Spi *spi)
    {
        this->cs  = cs;
        this->spi = spi;
    }
    void begin(uint8_t dev_num);
    void read_id(uint16_t *id);
    void read(uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
    void fast_read(uint8_t *buf, uint32_t read_addr, uint16_t num_to_read);
    void write(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);

    void erase_sector(uint32_t dst_addr);
    void erase_chip(void);


private:
    uint8_t                  spi_flash_buf[4096];
    SPI_CONFIG_TYPE     spi_dev_w25x16;
    Gpio                *cs;
    Spi                 *spi;

    uint8_t   readSR(void);
    void _waitBusy(void);
    void powerDown(void);
    void wakeUp(void);
    void writeSR(uint8_t sr);
    void writeEnable(void);
    void writeDisable(void);

    void write_page(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);
    void write_no_check(uint8_t *buf, uint32_t write_addr, uint16_t num_to_write);


};
#endif
