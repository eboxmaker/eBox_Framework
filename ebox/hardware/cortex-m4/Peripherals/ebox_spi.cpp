
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
#include "stm32f4xx_spi.h"


Spi::Spi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    busy = 0;
    spi = SPIx;	
    this->sck = sck;
    this->miso = miso;
    this->mosi = mosi;
	
}  

void Spi::begin(SPI_CONFIG_TYPE *spi_config)
{
//	  sck->mode(AF_PP_PU,GPIO_AF_SPI3);
//    miso->mode(AF_PP_PU,GPIO_AF_SPI3);
//    mosi->mode(AF_PP_PU,GPIO_AF_SPI3);
//	
    if(spi == SPI1)
    {
        sck->mode(AF_PP_PU,GPIO_AF_SPI1);
        miso->mode(AF_PP_PU,GPIO_AF_SPI1);
        mosi->mode(AF_PP_PU,GPIO_AF_SPI1);

        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    }
    if(spi == SPI2)
    {
        sck->mode(AF_PP_PU,GPIO_AF_SPI2);
        miso->mode(AF_PP_PU,GPIO_AF_SPI2);
        mosi->mode(AF_PP_PU,GPIO_AF_SPI2);

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    }
    if(spi == SPI3)
    {
        sck->mode(AF_PP_PU,GPIO_AF_SPI3);
        miso->mode(AF_PP_PU,GPIO_AF_SPI3);
        mosi->mode(AF_PP_PU,GPIO_AF_SPI3);

        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    }

    config(spi_config);
}
void Spi::config(SPI_CONFIG_TYPE *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;

    current_dev_num = spi_config->dev_num;


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
    SPI_InitStructure.SPI_BaudRatePrescaler = spi_config->prescaler;
    SPI_InitStructure.SPI_FirstBit = spi_config->bit_order;
    SPI_Init(spi, &SPI_InitStructure);
    SPI_Cmd(spi, ENABLE);
}

uint8_t Spi::read_config(void)
{
    return current_dev_num;
}


uint8_t Spi::transfer(uint8_t data)
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = data;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return spi->DR;
}


int8_t Spi::write(uint8_t data)
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
int8_t Spi::write(uint8_t *data, uint16_t data_length)
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
uint8_t Spi::read()
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = 0xff;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return(spi->DR);

}
int8_t Spi::read(uint8_t *recv_data)
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = 0xff;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    *recv_data = spi->DR;

    return 0;
}

int8_t Spi::read(uint8_t *recv_data, uint16_t data_length)
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

int8_t Spi::take_spi_right(SPI_CONFIG_TYPE *spi_config)
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
int8_t Spi::release_spi_right(void)
{
    busy = 0;
    return 0;
}



