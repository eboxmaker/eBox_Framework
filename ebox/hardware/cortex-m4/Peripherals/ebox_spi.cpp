
/*
file   : _spi.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "ebox_spi.h"

mcuSpi::mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    _busy = 0;
    _spi = SPIx;
    _sck = sck;
    _miso = miso;
    _mosi = mosi;

}

void mcuSpi::begin(Config_t *spi_config)
{

    if(_spi == SPI1)
    {
        _sck->mode(AF_PP_PU, GPIO_AF_SPI1);
        _miso->mode(AF_PP_PU, GPIO_AF_SPI1);
        _mosi->mode(AF_PP_PU, GPIO_AF_SPI1);

    }
    if(_spi == SPI2)
    {
        _sck->mode(AF_PP_PU, GPIO_AF_SPI2);
        _miso->mode(AF_PP_PU, GPIO_AF_SPI2);
        _mosi->mode(AF_PP_PU, GPIO_AF_SPI2);

    }
    if(_spi == SPI3)
    {
        _sck->mode(AF_PP_PU, GPIO_AF_SPI3);
        _miso->mode(AF_PP_PU, GPIO_AF_SPI3);
        _mosi->mode(AF_PP_PU, GPIO_AF_SPI3);

    }

    rcc_clock_cmd((uint32_t)_spi, ENABLE);
    config(spi_config);
}
void mcuSpi::config(Config_t *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;

    current_dev_num = spi_config->dev_num;

    SPI_Cmd(_spi, DISABLE);

    SPI_I2S_DeInit(_spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式

    if(spi_config->mode == MODE0)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(spi_config->mode == MODE1)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    else if(spi_config->mode == MODE2)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(spi_config->mode == MODE3)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    switch(spi_config->prescaler)
    {
    case DIV2:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        break;
    case DIV4:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
        break;
    case DIV8:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        break;
    case DIV16:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
        break;
    case DIV32:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
        break;
    case DIV64:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        break;
    case DIV128:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
        break;
    case DIV256:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;
    default :
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;

    }

    switch(spi_config->bit_order)
    {
    case MSB:
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        break;
    case LSB:
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
        break;
    default :
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        break;

    }
    SPI_Init(_spi, &SPI_InitStructure);
    SPI_Cmd(_spi, ENABLE);
}

uint8_t mcuSpi::read_config(void)
{
    return current_dev_num;
}


/**
  *@brief    transfer
  *@param    data 要发送的数据
  *@retval   uint8_t 接受到的数据
  */
uint8_t mcuSpi::transfer(uint8_t data)
{
    while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    _spi->DR = data;
    while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return _spi->DR;
}

/**
  *@brief    写数据
  *@param    data 要写入的数据
  *@retval   none
  */
int8_t mcuSpi::write(uint8_t data)
{
    __IO uint8_t dummyByte;
    while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    _spi->DR = data;
    while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    dummyByte = _spi->DR;

    return 0;
}
/**
  *@brief    连续写数据
  *@param    data 要写入的数据，data_length 长度
  *@retval   none
  */
int8_t mcuSpi::write_buf(uint8_t *data, uint16_t len)
{
    __IO uint8_t dummyByte;
    if(len == 0)
        return -1;
    while(len--)
    {
        while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
            ;
        _spi->DR = *data++;
        while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
            ;
        dummyByte = _spi->DR;
    }
    return 0;
}
uint8_t mcuSpi::read()
{
    while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    _spi->DR = 0xff;
    while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return(_spi->DR);

}
int8_t mcuSpi::read(uint8_t *recv_data)
{
    while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    _spi->DR = 0xff;
    while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    *recv_data = _spi->DR;

    return 0;
}

/**
  *@brief    连续读取数据
  *@param    recv_data 读取到的数据，data_length 要读取的数据长度
  *@retval   返回当前设备id
  */
int8_t mcuSpi::read_buf(uint8_t *recv_data, uint16_t len)
{
    if(len == 0)
        return -1;
    while(len--)
    {
        while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
            ;
        _spi->DR = 0xff;
        while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
            ;
        *recv_data++ = _spi->DR;
    }
    return 0;
}

/**
  *@brief    获取控制权
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::take(Config_t *newConfig)
{

    uint32_t end = GetEndTime(200);

    while (_busy == 1)
    {
        delay_ms(1);
        if (IsTimeOut(end, 200))
        {
            ebox_printf("\r\nSPI产生多线程异常调用\r\n");
            return EWAIT;
        }
    }
    if (newConfig->dev_num != read_config()) 
        config(newConfig);
    _busy = 1;
    return EOK;
}
/**
  *@brief    释放控制权
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::release(void)
{
    _busy = 0;
    return 0;
}



