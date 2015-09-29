#ifndef __20C02_h
#define __20C02_h
#include "ebox.h"

#define AT24C02 
#define SLAVE_ADDR 0xA0

#if defined (AT24C02) 
  #define PAGE_SIZE    8
#elif defined (AT24C04)||(AT24C08)||(AT24C016) 
  #define PAGE_SIZE    16
#elif defined (AT24C32)
  #define PAGE_SIZE    32
#endif

class EEPROM
{
	public:
		EEPROM(SOFTI2C* pI2C)
		{
		i2c = pI2C;
		}
		void begin(uint32_t speed)
		{
			i2c->begin(speed);
		}
		int8_t write_byte(uint16_t byteAddr,uint8_t byte);
		int8_t write_byte(uint16_t byteAddr,uint8_t* buf,uint16_t numToWrite);
		
		uint8_t read_byte(uint16_t byteAddr);
		int8_t	read_byte(uint16_t byteAddr,uint8_t* buf,uint16_t numToRead);
		
		int8_t write_page(uint8_t* buf,uint32_t addr,uint8_t numToWrite);	

		
	private:
		SOFTI2C* i2c;
		uint8_t slaveAddr;
		

};

#endif
