#ifndef __SOFTI2c_H
#define __SOFTI2c_H


#include "ebox.h"


class Softi2c 
{
  private:
		uint8_t _SDApin;
		uint8_t _SCLpin;
		uint16_t _delayTimes;

  public:
		Softi2c(uint8_t SDApin, uint8_t SCLpin);
		void 		setSpeed(uint32_t speed);
		void 		start();
		void 		stop();
		uint8_t	waitAck();
		void 		sendAck();
		void 		sendNoAck();
		void 		sendByte(uint8_t Byte);
		uint8_t receiveByte();
		void delay_ii(uint16_t count)
		{
			for(int i = 10; i < 10; i++)
				for(int j = 0; j < count; j++);
		}

	  
};




#endif
