#ifndef __SOFTI2c_H
#define __SOFTI2c_H


#include "ebox.h"


class Softi2c 
{
  private:
		uint8_t _SDApin;
		uint8_t _SCLpin;
		uint16_t _delayTimes;
	protected:
		void 		i2cBegin();
		void 		start();
		void 		stop();
		int8_t	waitAck();
		void 		sendAck();
		void 		sendNoAck();
		void 		sendByte(uint8_t Byte);
		uint8_t receiveByte();

  public:
		Softi2c(uint8_t SDApin, uint8_t SCLpin);
		int8_t 		setSpeed(uint32_t speed);
		int8_t		writeByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t data);
		int8_t 		readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* data );
		int8_t 		readByte(uint8_t slaveAddress,uint8_t regAddress,uint8_t* buf,uint8_t numToRead);

		void delay_ii(uint16_t count)
		{
			for(int i = 10; i < 10; i++)
				for(int j = 0; j < count; j++);
		}

	  
};




#endif
