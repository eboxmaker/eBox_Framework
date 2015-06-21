#ifndef __SPI_H
#define __SPI_H
#include "ebox.h"


#define SPI1_MOSI_PIN  7
#define SPI1_MISO_PIN  6
#define SPI1_SCK_PIN   5

#define SPI2_MOSI_PIN  31
#define SPI2_MISO_PIN  30
#define SPI2_SCK_PIN   29


#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03


#define SPI_CLOCK_DIV2     SPI_BaudRatePrescaler_2  
#define SPI_CLOCK_DIV4     SPI_BaudRatePrescaler_4  
#define SPI_CLOCK_DIV8     SPI_BaudRatePrescaler_8  
#define SPI_CLOCK_DIV16    SPI_BaudRatePrescaler_16 
#define SPI_CLOCK_DIV32    SPI_BaudRatePrescaler_32 
#define SPI_CLOCK_DIV64    SPI_BaudRatePrescaler_64 
#define SPI_CLOCK_DIV128   SPI_BaudRatePrescaler_128
#define SPI_CLOCK_DIV256   SPI_BaudRatePrescaler_256

#define SP1_BITODER_MSB		SPI_FirstBit_MSB                
#define SP1_BITODER_LSB		SPI_FirstBit_LSB                


class	SPIClass 
{
public:
	SPIClass(	SPI_TypeDef *spi)
	{
		_spi = spi;
		_prescaler = SPI_CLOCK_DIV256;
		_mode = SPI_MODE0;
		_bitOrder = SP1_BITODER_MSB;
		

	};
	void config();
	void begin();
	void setMode(uint8_t spimode);
	void setClockDivider( uint8_t divider);
	void setBitOrder(uint16_t bitOrder);
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
		if(dataln == 0)
			return;
		while(dataln--)
		{
			while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
				;
			_spi->DR = *data++;
			while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
				;
		}
	}

	void transfer(uint8_t *data,uint8_t *rcvdata,uint16_t dataln) 
	{
		if(dataln == 0)
			return;
		while(dataln--)
		{
			while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
				;
			_spi->DR = *data++;
			while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
				;
			*rcvdata++ = _spi->DR;
		}
	}
	private:
	
	SPI_TypeDef *_spi;
	SPI_InitTypeDef SPI_InitStructure;
	uint8_t _mode;
	uint8_t _prescaler;
	uint8_t _bitOrder;
	
	uint8_t _initialized;
};
extern SPIClass SPI;

#endif
