#include "spi.h"
void SPIClass::begin()
{
	pinMode(SPI1_SCK_PIN,AF_PP);
	pinMode(SPI1_MISO_PIN,AF_PP);
	pinMode(SPI1_MOSI_PIN,AF_PP);
	config();
	setClockDivider(_prescaler);
	setMode(_mode);
	setBitOrder(_bitOrder);
}
void SPIClass::config()
{
	if (_initialized)
		return;
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

	SPI_I2S_DeInit(_spi);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; //空闲模式下SCK为1
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; //数据采样从第2个时间边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128; //波特率
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //大端模式
	SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式
	SPI_Init(_spi, &SPI_InitStructure);
	SPI_Cmd(_spi, ENABLE);
	_initialized = 1;

}
void SPIClass::setMode(uint8_t spimode)
{

	_mode = spimode;
	
	
	SPI_Cmd(_spi,DISABLE);
	if(spimode == SPI_MODE0)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} else if(spimode == SPI_MODE1)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	} else if(spimode == SPI_MODE2)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	} else if(spimode == SPI_MODE3)
	{
	  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	}
	
	SPI_Init(_spi,&SPI_InitStructure);
	SPI_Cmd(_spi,ENABLE);


}

void SPIClass::setClockDivider( uint8_t prescaler) {

	_prescaler = prescaler;
	SPI_Cmd(_spi,DISABLE);
	SPI_InitStructure.SPI_BaudRatePrescaler = prescaler;
	SPI_Init(_spi,&SPI_InitStructure);
	SPI_Cmd(_spi,ENABLE);
}

void SPIClass::setBitOrder( uint16_t bitOrder) {
	
	_bitOrder = bitOrder;
	
	SPI_Cmd(_spi,DISABLE);
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; //大端模式
	SPI_Init(_spi,&SPI_InitStructure);
	SPI_Cmd(_spi,ENABLE);
}

SPIClass SPI(SPI1);







