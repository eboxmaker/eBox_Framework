#ifndef __I2C_H
#define __I2C_H

#include "ebox.h"

class I2C
{
	public:
		void begin();
		void init();
		void start();
		void stop();
		void sendNoAck();
		void sendAck();

		void sendByte(uint8_t regData);
		void send7BitsAddress(uint8_t address);
		uint8_t receiveByte();
	
		void writeByte(uint8_t regAddress,uint8_t regData);
		int8_t readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint8_t numToRead);

		void mpuInit();
		int8_t getID(uint8_t* data);
	
		int16_t getData(uint8_t REG_Address);
		int8_t  getData(uint8_t regAddress,int16_t* buf,uint8_t numToRead);

	private:
		
};

#endif
