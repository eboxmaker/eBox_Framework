#include "spi.h"

//#define SPI_NUM 3



SPIClass::SPIClass(SPI_TypeDef *spi,GPIO* sckPin,GPIO* mosiPin,GPIO* misoPin)
{
	_spi = spi;
	_sckPin = sckPin;
	_mosiPin = mosiPin;
	_misoPin = misoPin;
	
};

void SPIClass::begin(SPICONFIG* spiConfig)
{		
	if(_spi == SPI1)
	{	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	}
	if(_spi == SPI2)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	}
	if(_spi == SPI3)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	}
	
	
	pMode(_sckPin,_AF_PP);
	pMode(_mosiPin,_AF_PP);
	pMode(_misoPin,_AF_PP);

	config(spiConfig);
}
void SPIClass::config(SPICONFIG* spiConfig)
{
	currentDevNum = spiConfig->devNum;
	SPI_Cmd(_spi,DISABLE);
	
	SPI_I2S_DeInit(_spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式

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
	SPI_Init(_spi,&SPI_InitStructure);
	SPI_Cmd(_spi,ENABLE);

}

uint8_t SPIClass::readConfig(void)
{
	return currentDevNum; 
}

uint8_t SPIClass::transfer(uint8_t data) 
{
	while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
	;
	_spi->DR = data;
	while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
	;
	return (_spi->DR);
}

void SPIClass::transfer(uint8_t *data,uint16_t dataln) 
{
	__IO uint8_t dummyByte;
	if(dataln == 0)
		return;
	while(dataln--)
	{
		while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
			;
		_spi->DR = *data++;
		while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
			;
		dummyByte = _spi->DR;
	}
}


void SPIClass::transfer(uint8_t dummyByte,uint8_t *rcvdata,uint16_t dataln) 
{
	if(dataln == 0)
		return;
	while(dataln--)
	{
		while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
			;
		_spi->DR = dummyByte;
		while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
			;
		*rcvdata++ = _spi->DR;
	}
}





