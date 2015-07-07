/*
file   : 74hc595.c
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

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
void _74hc595::rowOut(uint8_t* Data,uint8_t dataLen)
{

	digitalWrite(_rckPin, LOW);
	for(int i = 0; i < dataLen; i++)
	shiftOut(_dataPin,_sckPin,MSBFIRST,Data[i]);
	digitalWrite(_rckPin, HIGH);
}
