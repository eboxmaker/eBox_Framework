#ifndef __LED_H
#define __LED_H
#include "ebox.h"
#include "74hc595.h"

class LED:public _74hc595
{
	private:
		uint8_t _dataPin;
		uint8_t _sckPin;
		uint8_t _rckPin;
		uint8_t _LEDBuf[8];

	public:
		LED(uint8_t dataPin, uint8_t sckPin,uint8_t rckPin):_74hc595(dataPin,sckPin,rckPin)
		{
			
		};
		void ShowTimeHMS(uint8_t hour,uint8_t min,uint8_t sec);
		void ShowDateTime(uint16_t year,uint8_t month,uint8_t date);

};
#endif
