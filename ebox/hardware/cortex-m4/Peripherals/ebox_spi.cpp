
/*
file   : spi.cpp
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
#include "ebox_gpio.h"

mcuSpi::mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    busy = 0;
    spi = SPIx;	
    this->sck = sck;
    this->miso = miso;
    this->mosi = mosi;
	
}  

void mcuSpi::begin(SpiConfig_t *spi_config)
{

    if(spi == SPI1)
    {
        sck->mode(AF_PP_PU,GPIO_AF_SPI1);
        miso->mode(AF_PP_PU,GPIO_AF_SPI1);
        mosi->mode(AF_PP_PU,GPIO_AF_SPI1);

    }
    if(spi == SPI2)
    {
        sck->mode(AF_PP_PU,GPIO_AF_SPI2);
        miso->mode(AF_PP_PU,GPIO_AF_SPI2);
        mosi->mode(AF_PP_PU,GPIO_AF_SPI2);

    }
    if(spi == SPI3)
    {
        sck->mode(AF_PP_PU,GPIO_AF_SPI3);
        miso->mode(AF_PP_PU,GPIO_AF_SPI3);
        mosi->mode(AF_PP_PU,GPIO_AF_SPI3);

    }

    
    config(spi_config);
}
void mcuSpi::config(SpiConfig_t *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;

    current_dev_num = spi_config->dev_num;

    rcc_clock_cmd((uint32_t)spi,ENABLE);
    SPI_Cmd(spi, DISABLE);

    SPI_I2S_DeInit(spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式

    if(spi_config->mode == SPI_MODE0)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(spi_config->mode == SPI_MODE1)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    else if(spi_config->mode == SPI_MODE2)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(spi_config->mode == SPI_MODE3)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    switch(spi_config->prescaler)
    {
        case SPI_CLOCK_DIV2:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;break;
        case SPI_CLOCK_DIV4:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;break;
        case SPI_CLOCK_DIV8:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;break;
        case SPI_CLOCK_DIV16:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;break;
        case SPI_CLOCK_DIV32:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;break;
        case SPI_CLOCK_DIV64:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;break;
        case SPI_CLOCK_DIV128:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;break;
        case SPI_CLOCK_DIV256:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;break;
        default :
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;break;

    }
    
    switch(spi_config->bit_order)
    {
        case MSB_FIRST:
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;break;
        case LSB_FIRST:
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;break;
        default :
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;break;

    }
    SPI_Init(spi, &SPI_InitStructure);
    SPI_Cmd(spi, ENABLE);
}

uint8_t mcuSpi::read_config(void)
{
    return current_dev_num;
}


uint8_t mcuSpi::transfer(uint8_t data)
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = data;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return spi->DR;
}


int8_t mcuSpi::write(uint8_t data)
{
    __IO uint8_t dummyByte;
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = data;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    dummyByte = spi->DR;

    return 0;
}
int8_t mcuSpi::write(uint8_t *data, uint16_t data_length)
{
    __IO uint8_t dummyByte;
    if(data_length == 0)
        return -1;
    while(data_length--)
    {
        while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
            ;
        spi->DR = *data++;
        while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
            ;
        dummyByte = spi->DR;
    }
    return 0;
}
uint8_t mcuSpi::read()
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = 0xff;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return(spi->DR);

}
int8_t mcuSpi::read(uint8_t *recv_data)
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = 0xff;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    *recv_data = spi->DR;

    return 0;
}

int8_t mcuSpi::read(uint8_t *recv_data, uint16_t data_length)
{
    if(data_length == 0)
        return -1;
    while(data_length--)
    {
        while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
            ;
        spi->DR = 0xff;
        while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
            ;
        *recv_data++ = spi->DR;
    }
    return 0;
}

int8_t mcuSpi::take_spi_right(SpiConfig_t *spi_config)
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
int8_t mcuSpi::release_spi_right(void)
{
    busy = 0;
    return 0;
}



