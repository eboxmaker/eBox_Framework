/*
file   : spi.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "spi.h"


SPI::SPI(SPI_TypeDef *SPIx,GPIO* p_sck_pin,GPIO* p_miso_pin,GPIO* p_mosi_pin)
{
	busy = 0;
	spi = SPIx;
	p_sck_pin->mode(AF_PP);
	p_miso_pin->mode(AF_PP);
	p_mosi_pin->mode(AF_PP);
	
};

void SPI::begin(SPI_CONFIG_TYPE* spi_config)
{		
	if(spi == SPI1)
	{	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	}
	if(spi == SPI2)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	}
	if(spi == SPI3)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	}

	config(spi_config);
}
void SPI::config(SPI_CONFIG_TYPE* spi_config)
{
	SPI_InitTypeDef SPI_InitStructure;

	current_dev_num = spi_config->dev_num;
	
	
	SPI_Cmd(spi,DISABLE);
	
	SPI_I2S_DeInit(spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����ģʽ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�������
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC����ʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����ģʽ

	if(spi_config->mode == SPI_MODE0)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} else if(spi_config->mode == SPI_MODE1)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	} else if(spi_config->mode == SPI_MODE2)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} else if(spi_config->mode == SPI_MODE3)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStructure.SPI_BaudRatePrescaler = spi_config->prescaler;
	SPI_InitStructure.SPI_FirstBit = spi_config->bit_order; 
	SPI_Init(spi,&SPI_InitStructure);
	SPI_Cmd(spi,ENABLE);

}

uint8_t SPI::read_config(void)
{
	return current_dev_num; 
}


int8_t SPI::write(uint8_t data)
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
int8_t SPI::write(uint8_t *data,uint16_t data_length)
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
uint8_t SPI::read()
{
	while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
	;
	spi->DR = 0xff;
	while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
	;
	return(spi->DR);
	
}
int8_t SPI::read(uint8_t* recv_data)
{
	while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
	;
	spi->DR = 0xff;
	while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
	;
	*recv_data = spi->DR;
	
	return 0;
}

int8_t SPI::read(uint8_t *recv_data,uint16_t data_length)
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

int8_t SPI::take_spi_right(SPI_CONFIG_TYPE* spi_config)
{
	while((busy == 1)&&(spi_config->dev_num != read_config()))
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
int8_t SPI::release_spi_right(void)
{
	busy = 0;
	return 0;
}



