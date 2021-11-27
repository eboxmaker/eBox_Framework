/**
  ******************************************************************************
  * @file    spi.h
  * @author  shentq
  * @version V2.1
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

#ifndef __SPI_H
#define __SPI_H

#include "ebox_core.h"
#include "spi.h"
#include "mcu.h"



/*
	1.目前只测试了SPI1、SPI2，spi3望网友测试
	2.该spi功能强大，总线支持同时挂载不同MODE ,SPEED,bit_oder的设备
	3.每一个spi设备应有一个自己的SPI_CONFIG的配置，以支持该设备的的读写，
		在读写前需要获得SPI的控制权，如果获取不到则一直等待！主要是为了兼容操作系统，
		如果不使用操作系统也必须加上获得控制权的代码，在是用完SPI后一定要释放SPI总线，
		如果不释放总线会导致别的SPI设备一直处于等待的状态
*/
//默认配置 空，只依靠结构体SPICONFIG来初始化
class mcuSpi: public Spi
{
public:
    mcuSpi(SPI_Module *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

    virtual void    begin (Config_t *newConfig);
    virtual void    config(Config_t *newConfig);
    virtual uint8_t read_config(void);

    virtual uint8_t transfer(uint8_t data);

    virtual int8_t  write(uint8_t data);
    virtual uint8_t read();

    virtual int8_t  write_buf(uint8_t *data, uint16_t len);
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t len);


public:
    virtual int8_t  take(Config_t *newConfig);
    virtual int8_t  release(void);

private:
    uint8_t tx_buffer[1];
    uint8_t rx_buffer[1];

private:
    SPI_Module *_spi;
    Gpio        *_sck;
    Gpio        *_miso;
    Gpio        *_mosi;

    uint8_t     _busy;

};

#endif
