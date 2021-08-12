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

#ifndef __SOFT_SPI_H
#define __SOFT_SPI_H

#include "ebox_core.h"
#include "spi.h"



/*
	注意：1.该类的SPI_CLOCK_DIV是由delay_us延时函数控制。略有不准，比硬件SPI会慢很多
				2.speed设置只能为SPI_CLOCK_DIVx。如果不是此值，则会将SPI_CLOCK_DIV的值直接传递给delay_us.即delay_us(SPI_CONFIG_TYPE->prescaler);
				3.初期调试I2C设备建议使用SPI_CLOCK_DIV256。
				4.函数接口和硬件SPI完全一样可以互相替换。
*/
class SoftSpi: public Spi
{
public:
    SoftSpi(Gpio *sck, Gpio *miso, Gpio *mosi);

    virtual void    begin (Config_t *spi_config);
    virtual void    config(Config_t *spi_config);
    virtual uint8_t read_config(void);

    virtual uint8_t transfer(uint8_t data);

    virtual int8_t  write(uint8_t data);
    virtual uint8_t read();
    virtual int8_t  read(uint8_t  *recv_data);

    virtual int8_t  write_buf(uint8_t *data, uint16_t len);
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t len);

public:
    virtual int8_t take(Config_t *spi_config);
    virtual int8_t release(void);

private:
    Gpio    *_sck;
    Gpio    *_miso;
    Gpio    *_mosi;
    uint8_t _busy;

    Spi::Mode_t mode;
    Spi::BitOrder_t bit_order;
    uint8_t spi_delay;


    uint8_t transfer0(uint8_t data);
    uint8_t transfer1(uint8_t data);
    uint8_t transfer2(uint8_t data);
    uint8_t transfer3(uint8_t data);
};




#endif
