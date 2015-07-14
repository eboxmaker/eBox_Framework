/*
file   : spi.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "spi.h"

//#define SPI_NUM 3
uint8_t SPIClASS::currentDevNum = 0;

SPIClASS::SPIClASS(SPI_TypeDef *SPIx,GPIO* sckPin,GPIO* misoPin,GPIO* mosiPin)
{
	spi = SPIx;
	sckPin->mode(AF_PP);
	mosiPin->mode(AF_PP);
	misoPin->mode(AF_PP);
	
};

void SPIClASS::begin(SPICONFIG* spiConfig)
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

	config(spiConfig);
}
void SPIClASS::config(SPICONFIG* spiConfig)
{
	SPI_InitTypeDef SPI_InitStructure;

	currentDevNum = spiConfig->devNum;
	SPI_Cmd(spi,DISABLE);
	
	SPI_I2S_DeInit(spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����ģʽ
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�������
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC����ʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����ģʽ

	if(spiConfig->mode == SPI_MODE0)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} else if(spiConfig->mode == SPI_MODE1)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	} else if(spiConfig->mode == SPI_MODE2)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} else if(spiConfig->mode == SPI_MODE3)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	}
	SPI_InitStructure.SPI_BaudRatePrescaler = spiConfig->prescaler;
	SPI_InitStructure.SPI_FirstBit = spiConfig->bitOrder; 
	SPI_Init(spi,&SPI_InitStructure);
	SPI_Cmd(spi,ENABLE);

}

uint8_t SPIClASS::readConfig(void)
{
	return currentDevNum; 
}

uint8_t SPIClASS::transfer(uint8_t data) 
{
	while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
	;
	spi->DR = data;
	while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
	;
	return (spi->DR);
}

void SPIClASS::transfer(uint8_t *data,uint16_t dataln) 
{
	__IO uint8_t dummyByte;
	if(dataln == 0)
		return;
	while(dataln--)
	{
		while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
			;
		spi->DR = *data++;
		while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
			;
		dummyByte = spi->DR;
	}
}


void SPIClASS::transfer(uint8_t dummyByte,uint8_t *rcvdata,uint16_t dataln) 
{
	if(dataln == 0)
		return;
	while(dataln--)
	{
		while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
			;
		spi->DR = dummyByte;
		while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
			;
		*rcvdata++ = spi->DR;
	}
}





