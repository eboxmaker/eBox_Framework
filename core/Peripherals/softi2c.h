#ifndef __SOFTI2c_H
#define __SOFTI2c_H


#include "ebox.h"


class Softi2c 
{
  private:
		uint8_t _SDApin;
		uint8_t _SCLpin;
		uint16_t _delaytimes;

  public:
		Softi2c(uint8_t SDApin, uint8_t SCLpin);
		void 		start();
		void 		stop();
		uint8_t	wait_ack();
		void 		send_ack();
		void 		send_no_ack();
		void 		send_byte(uint8_t Byte);
		uint8_t read_byte();
		void delay_ii(uint16_t count)
		{
			for(int i = 10; i < 10; i++)
				for(int j = 0; j < count; j++);
		}

	  
};




#endif
