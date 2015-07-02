#ifndef __I2C_H
#define __I2C_H

#include "ebox.h"

class I2C
{
	private:
		uint8_t _SDAPin;
		uint8_t _SCLPin;
		 /* Initialize the I2C1 according to the I2C_InitStructure members */ 
		I2C_InitTypeDef I2C_InitStructure; 
		int8_t start();
		int8_t stop();
		int8_t sendNoAck();
		int8_t sendAck();

		int8_t sendByte(uint8_t regData);
		int8_t send7BitsAddress(uint8_t address);
		int8_t receiveByte(uint8_t* data);

	public:
		I2C(uint8_t SDAPin,uint8_t SCLPin)
		{
			_SDAPin = SDAPin;
			_SCLPin = SCLPin;

		}
		void i2cBegin();
		void setSpeed(uint32_t speed);
	
		int8_t writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t regData);
		int8_t readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data);
		int8_t readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data,uint8_t numToRead);

	private:
		
};

#endif
