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
void SoftSpi::begin(Config_t *newConfig)
{

    _sck->mode(OUTPUT_PP);
    _miso->mode(INPUT);
    _mosi->mode(OUTPUT_PP);

    config(newConfig);
    switch(mode)
    {
    case MODE0:
        _sck->reset();
        break;
    case MODE1:
        _sck->reset();
        break;
    case MODE2:
        _sck->set();
        break;
    case MODE3:
        _sck->set();
        break;
    }
}
void SoftSpi::config(Config_t *newConfig)
{
    current_dev_num = newConfig->dev_num;
    mode = newConfig->mode;
    bit_order = newConfig->bit_order;
    switch(newConfig->prescaler)
    {
    case DIV2:
        spi_delay = 0;
        break;
    case DIV4:
        spi_delay = 1;
        break;
    case DIV8:
        spi_delay = 2;
        break;
    case DIV16:
        spi_delay = 4;
        break;
    case DIV32:
        spi_delay = 8;
        break;
    case DIV64:
        spi_delay = 16;
        break;
    case DIV128:
        spi_delay = 32;
        break;
    case DIV256:
        spi_delay = 64;
        break;
    default:
        spi_delay = newConfig->prescaler;
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
        if (bit_order == LSB)
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
    case MODE0:
        RcvData = transfer0(data);
        break;
    case MODE1:
        RcvData = transfer1(data);
        break;
    case MODE2:
        RcvData = transfer2(data);
        break;
    case MODE3:
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

int8_t SoftSpi::take(Config_t *newConfig)
{

    uint32_t end = GetEndTime(200);

    while (_busy == 1)
    {
        delay_ms(1);
        if (IsTimeOut(end, 200))
        {
            ebox_printf("\r\nSPI�������߳��쳣����\r\n");
            return EWAIT;
        }
    }
    if (newConfig->dev_num != read_config()) 
        config(newConfig);
    _busy = 1;
    return EOK;
}
int8_t SoftSpi::release(void)
{
    _busy = 0;
    return 0;
}




