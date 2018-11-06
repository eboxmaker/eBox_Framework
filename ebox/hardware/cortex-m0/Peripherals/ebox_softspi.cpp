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

#include "ebox_config.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 1
#endif

#if debug
#define  SPI_DEBUG(...) DBG("[SPI]  "),DBG(__VA_ARGS__)
#else
#define  SPI_DEBUG(...)
#endif

SoftSpi::SoftSpi(Gpio *sck, Gpio *miso, Gpio *mosi)
{
    _sck =  sck;
    _miso = miso;
    _mosi = mosi;
}
void SoftSpi::begin(SpiConfig_t *spi_config)
{
    _sck->mode(OUTPUT_PP);
    _miso->mode(INPUT);
    _mosi->mode(OUTPUT_PP);

    config(spi_config);
    switch(mode)
    {
    case SPI_MODE0:
        _sck->reset();
        break;
    case SPI_MODE1:
        _sck->reset();
        break;
    case SPI_MODE2:
        _sck->set();
        break;
    case SPI_MODE3:
        _sck->set();
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
    //ʱ�ӿ��������0,��һ�����أ��������أ������֮�������
    //MOSI,�ڵ�һ��ʱ����֮ǰ׼�����������
    //MISO,�����ڵ�һ��ʱ����֮���ȡ���ݡ�

    for (i = 0; i < 8; i++)
    {
        ///////////////////������֮ǰ����׼������//////////
        if (bit_order == LSB_FIRST)
        {
            _mosi->write(!!(data & (1 << i)));
        }
        else
        {
            _mosi->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//����һ��ʱ�䣬��֤�����źŵ���ӻ�
        /////////////////������֮�������������ݣ�֪ͨ�ӻ���������֮ǰ׼���õ����ݣ�////////////
        _sck->set();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= _miso->read() << i;
        }
        else
        {
            RcvData |= (_miso->read() << (7 - i));
        }
        delay_us(spi_delay);
        _sck->reset();
    }
    return RcvData;
}
uint8_t SoftSpi::transfer1(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //CPOL = 0; CPHA = 1
    //ʱ�ӿ��������0,�ڶ������أ����½��أ������֮�������
    //MOSI,�ڵڶ���ʱ����֮ǰ׼�����������
    //MISO,�����ڵڶ���ʱ����֮���ȡ���ݡ�


    for (i = 0; i < 8; i++)
    {
        ///////////////////������֮������׼������//////////
        _sck->set();
        if (bit_order == LSB_FIRST)
        {
            _mosi->write(!!(data & (1 << i)));
        }
        else
        {
            _mosi->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//����һ��ʱ�䣬��֤���������źŵ���ӻ���
        /////////////////�½���֮�������������ݣ�֪ͨ�ӻ���������֮ǰ׼���õ����ݣ�////////////
        _sck->reset();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= _miso->read() << i;
        }
        else
        {
            RcvData |= (_miso->read() << (7 - i));
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
    //ʱ�ӿ��������1,��һ�����أ����½��أ������֮�������
    //MOSI,�����ڵ�һ��ʱ����֮ǰ׼�����������
    //MISO,�����ڵ�һ��ʱ����֮���ȡ���ݡ�
    for (i = 0; i < 8; i++)
    {
        ///////////////////�½���֮ǰ����׼������//////////
        if (bit_order == LSB_FIRST)
        {
            _mosi->write(!!(data & (1 << i)));
        }
        else
        {
            _mosi->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//����һ��ʱ�䣬��֤���������źŵ���ӻ���
        /////////////////�½���֮�������������ݣ�֪ͨ�ӻ���������֮ǰ׼���õ����ݣ�////////////
        _sck->reset();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= _miso->read() << i;
        }
        else
        {
            RcvData |= (_miso->read() << (7 - i));
        }
        delay_us(spi_delay);
        _sck->set();
    }

    return RcvData;
}
uint8_t SoftSpi::transfer3(uint8_t data)
{
    uint8_t i;
    uint8_t RcvData = 0 ;

    //CPOL = 1; CPHA = 1
    //ʱ�ӿ��������1,�ڶ������أ��������أ������֮�������
    //MOSI,�����ڵڶ���ʱ����֮ǰ׼�����������
    //MISO,�����ڵڶ���ʱ����֮���ȡ���ݡ�
    for (i = 0; i < 8; i++)
    {
        ///////////////////�½���֮������׼������//////////
        _sck->reset();
        if (bit_order == LSB_FIRST)
        {
            _mosi->write(!!(data & (1 << i)));
        }
        else
        {
            _mosi->write(!!(data & (1 << (7 - i))));
        }
        delay_us(spi_delay);//����һ��ʱ�䣬��֤���������źŵ���ӻ���
        /////////////////������֮�������������ݣ�֪ͨ�ӻ���������֮ǰ׼���õ����ݣ�////////////
        _sck->set();
        if (bit_order == LSB_FIRST)
        {
            RcvData |= _miso->read() << i;
        }
        else
        {
            RcvData |= (_miso->read() << (7 - i));
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
int8_t  SoftSpi::write_buf(uint8_t *data, uint16_t len)
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
int8_t  SoftSpi::read_buf(uint8_t *rcvdata, uint16_t len)
{
    if(len == 0)
        return -1;
    while(len--)
    {
        *rcvdata++ = transfer(0xff);
    }
    return 0;
}

int8_t SoftSpi::take(SpiConfig_t *spi_config)
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
int8_t SoftSpi::release(void)
{
    busy = 0;
    return 0;
}




