#ifndef __SOFTSPI_H
#define __SOFTSPI_H

#include "ebox.h"

#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

#define SP1_BITODER_MSB		SPI_FirstBit_MSB                
#define SP1_BITODER_LSB		SPI_FirstBit_LSB  

#define CPOL_Low	0
#define CPOL_High	2
#define CPHA_1Edge	0
#define CPHA_2Edge	1

class SOFTSPI
{
	public:
		SOFTSPI(uint8_t sckPin,uint8_t misoPin,uint8_t mosiPin);
		void begin();
		void setMode(uint8_t spimode);
		void setClockDivider( uint8_t prescaler);
		void setBitOrder(uint8_t bitOrder);
		
		uint8_t transfer(uint8_t data);
		
	private:
		uint8_t 	_sckPin;
		uint8_t		_mosiPin;
		uint8_t		_misoPin;
		
//		SPI_TypeDef *_spi;
//		SPI_InitTypeDef SPI_InitStructure;
	
		uint8_t _mode;
		uint8_t _cpol;
		uint8_t _cpha;
	
		uint8_t _bitOrder;
		
		uint8_t _spidelay;

		void spiDelay(uint32_t count);

};
#endif

