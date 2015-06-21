#ifndef __SOFTI2c_H
#define __SOFTI2c_H


#include "ebox.h"


class Softi2c 
{
  private:
		uint8_t _SDApin;
		uint8_t _SCLpin;
		uint16_t delaytimes;

  public:
		Softi2c(uint8_t SDApin, uint8_t SCLpin);
		void 		_start();
		void 		_stop();
		uint8_t		_wait_ack();
		void 		_send_ack();
		void 		_send_no_ack();
		void 		_send_byte(uint8_t Byte);
		uint8_t _read_byte();
		void delay_ii(uint16_t count)
		{
			for(int i = 10; i < 10; i++)
				for(int j = 0; j < count; j++);
		}

	  
};




#endif
