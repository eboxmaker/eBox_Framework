
#ifndef __IO_H
#define __IO_H
#include "common.h"

typedef enum
{ 
	_AIN = 0x0,
  _INPUT = 0x04,
  _IPD = 0x28,
  _IPU = 0x48,
  _OOD = 0x14,
  _OPP = 0x10,
  _AF_OD = 0x1C,
  _AF_PP = 0x18
}PINMODE;

#define dgWrite(pin,val)	pin->write(val)
#define dgRead(pin) 			pin->read()
#define pinMode(pin,val)	pin->mode(val)

class GPIO
{
	public:
		GPIO(GPIO_TypeDef* _port,uint16_t _pin);
		void mode(PINMODE modeVal);
		void write(uint8_t val);
		void read(uint8_t* val);	
		uint8_t read(void);
	private:
		GPIO_TypeDef* port;
		uint16_t pin;
		uint32_t rcc;
};

#endif

