#ifndef __SPI_H
#define __SPI_H
#include "ebox.h"


#define SPI1_MOSI_PIN  7
#define SPI1_MISO_PIN  6
#define SPI1_SCK_PIN   5

#define SPI2_MOSI_PIN  31
#define SPI2_MISO_PIN  30
#define SPI2_SCK_PIN   29






class	SPIClass 
{
public:
	SPIClass(SPI_TypeDef *spi)
	{
		_spi = spi;
	};
	void begin(SPICONFIG* spiConfig);
	void ioInit();
	void config(SPICONFIG* spiConfig);
	uint8_t readConfig(void);
	uint8_t transfer(uint8_t data) 
	{
	  while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
		;
	  _spi->DR = data;
	  while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
		;
	  return (_spi->DR);
	}

	void transfer(uint8_t *data,uint16_t dataln) 
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

	void transfer(uint8_t dummyByte,uint8_t *rcvdata,uint16_t dataln) 
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
	private:
	
	SPI_TypeDef *_spi;
	SPI_InitTypeDef SPI_InitStructure;

	uint8_t currentDevNum;
};
extern SPIClass spi1;

#endif
