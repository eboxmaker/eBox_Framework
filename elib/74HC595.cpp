#include "74hc595.h"

_74hc595::_74hc595(uint8_t dataPin, uint8_t sckPin,uint8_t rckPin)
{
	_dataPin 	= dataPin;
	_sckPin 	= sckPin;
	_rckPin 	= rckPin;
	 pinMode(_dataPin, OUTPUT);
	 pinMode(_sckPin, OUTPUT);
	 pinMode(_rckPin, OUTPUT);
}
void _74hc595::RowOut(uint8_t* Data,uint8_t dataLen)
{

	digitalWrite(_rckPin, LOW);
	for(int i = 0; i < dataLen; i++)
	shiftOut(_dataPin,_sckPin,MSBFIRST,Data[i]);
	digitalWrite(_rckPin, HIGH);
}
