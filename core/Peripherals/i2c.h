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
		uint8_t readByte(uint8_t regAddress);
		uint16_t readByte(uint8_t regAddress,uint8_t* buf,uint8_t numToRead);
		void mpuInit();
		int8_t getID();
	
		int16_t GetData(uint8_t REG_Address);
	private:
		
};

#endif
