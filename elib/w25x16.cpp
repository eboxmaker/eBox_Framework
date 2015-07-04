#include "w25x16.h"

SPICONFIG spiDevW25x16;

void W25X::begin()
{
	spiDevW25x16.devNum = 1;
	spiDevW25x16.mode = SPI_MODE0;
	spiDevW25x16.prescaler = SPI_CLOCK_DIV2;
	spiDevW25x16.bitOrder = SPI_BITODER_MSB;
	
	spi1.begin(&spiDevW25x16);
	pinMode(0x45,OUTPUT);
	digitalWrite(cs,HIGH);
}
void W25X::readId(uint16_t* id)
{
	if(spiDevW25x16.devNum != spi1.readConfig())
		spi1.config(&spiDevW25x16);

	digitalWrite(cs,LOW);
	spi1.transfer(0x90);
	spi1.transfer(0x00);
	spi1.transfer(0x00);
	spi1.transfer(0x00);
	*id |= spi1.transfer(0xff)<<8;
	*id |= spi1.transfer(0xff);
	digitalWrite(cs,HIGH);
	
	

}