/**
  ******************************************************************************
  * @file    softspi.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "ebox_spi.h"

SoftSpi::SoftSpi(Gpio *sck, Gpio *miso, Gpio *mosi)
{
    this->sck_pin =  sck;
    this->miso_pin = miso;
    this->mosi_pin = mosi;

}
void SoftSpi::begin(SpiConfig_t *spi_config)
{

    sck_pin->mode(OUTPUT_PP);
    miso_pin->mode(INPUT);
    mosi_pin->mode(OUTPUT_PP);

    config(spi_config);
    switch(mode)
    {
    case SPI_MODE0:
        sck_pin->reset();
        break;
    case SPI_MODE1:
        sck_pin->reset();
        break;
    case SPI_MODE2:
        sck_pin->set();
        break;
    case SPI_MODE3:
        sck_pin->set();
        break;
    }
}
void SoftSpi::config(SpiConfig_t *spi_config)
{
    current_dev_num = spi_config->dev_num;
    mode = spi_config->mode;
    bit_order = spi_config->bit_order;
    switch(spi_config->prescaler)
    {
    case SPI_CLOCK_DIV2:
        spi_delay = 0;
        break;
    case SPI_CLOCK_DIV4:
        spi_delay = 1;
        break;
    case SPI_CLOCK_DIV8:
        spi_delay = 2;
        break;
    case SPI_CLOCK_DIV16:
        spi_delay = 4;
        break;
    case SPI_CLOCK_DIV32:
        spi_delay = 8;
        break;
    case SPI_CLOCK_DIV64:
        spi_delay = 16;
        break;
    case SPI_CLOCK_DIV128:
        spi_delay = 32;
        break;
    case SPI_CLOCK_DIV256:
        spi_delay = 64;
        break;
    default:
        spi_delay = spi_config->prescaler;
        break;
    }
}
uint8_t SoftSpi::read_config(void)
{
    return current_dev_num;
}

uint8_t SoftSpi::transfer0(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //CPOL = 0; CPHA = 0
    //时钟空闲输出：0,第一个边沿（即上升沿）输出，之后采样；
    //MOSI,在第一个时钟沿之前准备好输出数据
    //MISO,主机在第一个时钟沿之后读取数据。

    for (i = 0; i < 8; i++)
    {
        ///////////////////上升沿之前主机准备数据//////////
        if (bit_order == LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//保持一段时间，保证主机信号到达从机
        /////////////////上升沿之后主机接收数据（通知从机接收主机之前准备好的数据）////////////
        sck_pin->set();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= miso_pin->read() << i;
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
        }
        delay_us(spi_delay);
        sck_pin->reset();
    }
    return RcvData;
}
uint8_t SoftSpi::transfer1(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //CPOL = 0; CPHA = 1
    //时钟空闲输出：0,第二个边沿（即下降沿）输出，之后采样；
    //MOSI,在第二个时钟沿之前准备好输出数据
    //MISO,主机在第二个时钟沿之后读取数据。


    for (i = 0; i < 8; i++)
    {
        ///////////////////上升沿之后主机准备数据//////////
        sck_pin->set();
        if (bit_order == LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//保持一段时间，保证主机发送信号到达从机。
        /////////////////下降沿之后主机接收数据（通知从机接收主机之前准备好的数据）////////////
        sck_pin->reset();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= miso_pin->read() << i;
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
        }
        delay_us(spi_delay);
    }

    return RcvData;
}
uint8_t SoftSpi::transfer2(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

     //CPOL = 1; CPHA = 0
    //时钟空闲输出：1,第一个边沿（即下降沿）输出，之后采样；
    //MOSI,主机在第一个时钟沿之前准备好输出数据
    //MISO,主机在第一个时钟沿之后读取数据。
    for (i = 0; i < 8; i++)
    {
        ///////////////////下降沿之前主机准备数据//////////
        if (bit_order == LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//保持一段时间，保证主机发送信号到达从机。
        /////////////////下降沿之后主机接收数据（通知从机接收主机之前准备好的数据）////////////
        sck_pin->reset();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= miso_pin->read() << i;
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
        }
        delay_us(spi_delay);
        sck_pin->set();
    }

    return RcvData;
}
uint8_t SoftSpi::transfer3(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

     //CPOL = 1; CPHA = 1
    //时钟空闲输出：1,第二个边沿（即上升沿）输出，之后采样；
    //MOSI,主机在第二个时钟沿之前准备好输出数据
    //MISO,主机在第二个时钟沿之后读取数据。
    for (i = 0; i < 8; i++)
    {
        ///////////////////下降沿之后主机准备数据//////////
        sck_pin->reset();
        if (bit_order == LSB_FIRST)
        {
            mosi_pin->write(!!(data & (1 << i)));
        }
        else
        {
            mosi_pin->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//保持一段时间，保证主机发送信号到达从机。
        /////////////////上升沿之后主机接收数据（通知从机接收主机之前准备好的数据）////////////
        sck_pin->set();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= miso_pin->read() << i;
        }
        else
        {
            RcvData |= (miso_pin->read() << (7 - i));
        }
        delay_us(spi_delay);
    }

    return RcvData;
}
uint8_t SoftSpi::transfer(uint8_t data)
{
    uint8_t RcvData = 0 ;
    switch(mode)
    {
    case SPI_MODE0:
        RcvData = transfer0(data);
        break;
    case SPI_MODE1:
        RcvData = transfer1(data);
        break;
    case SPI_MODE2:
        RcvData = transfer2(data);
        break;
    case SPI_MODE3:
        RcvData = transfer3(data);
        break;
    default :
        //return 0xff;
        break;
    }
    return RcvData;

}

int8_t  SoftSpi::write(uint8_t data)
{
    transfer(data);
    return 0;
}
int8_t  SoftSpi::write(uint8_t *data, uint16_t len)
{
    if(len == 0)
        return -1;
    while(len--)
    {
        transfer(*data++);
    }
    return 0;
}

uint8_t SoftSpi::read()
{
    return transfer(0xff);
}
int8_t  SoftSpi::read(uint8_t *data)
{
    *data = transfer(0xff);
    return 0;

}
int8_t  SoftSpi::read(uint8_t *rcvdata, uint16_t len)
{
    if(len == 0)
        return -1;
    while(len--)
    {
        *rcvdata++ = transfer(0xff);
    }
    return 0;
}

int8_t SoftSpi::take_spi_right(SpiConfig_t *spi_config)
{
    while((busy == 1) && (spi_config->dev_num != read_config()))
        delay_ms(1);
    if(spi_config->dev_num == read_config())
    {
        busy = 1;
        return 0;
    }
    config(spi_config);
    busy = 1;
    return 0;
}
int8_t SoftSpi::release_spi_right(void)
{
    busy = 0;
    return 0;
}




