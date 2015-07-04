#include "spi.h"

void SPIClass::begin(SPICONFIG* spiConfig)
{

	ioInit();
	config(spiConfig);
}
void SPIClass::ioInit()
{
	if(_spi == SPI1)
	{	
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
		
		pinMode(SPI1_SCK_PIN,AF_PP);
		pinMode(SPI1_MISO_PIN,AF_PP);
		pinMode(SPI1_MOSI_PIN,AF_PP);
	}
	if(_spi == SPI2)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	}
	if(_spi == SPI3)
	{	
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	}

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


SPIClass spi1(SPI1);







