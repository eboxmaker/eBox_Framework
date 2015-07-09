/*
file   : 74hc595.c
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "74hc595.h"

_74hc595::_74hc595(GPIO* dataPin, GPIO* sckPin,GPIO* rckPin)
{
	_dataPin 	= dataPin;
	_sckPin 	= sckPin;
	_rckPin 	= rckPin;
	
	_dataPin->mode(_OPP);
	_sckPin->mode(_OPP);
	_rckPin->mode(_OPP);
}
void _74hc595::rowOut(uint8_t* Data,uint8_t dataLen)
{

	_rckPin->write(LOW);
	for(int i = 0; i < dataLen; i++)
	shiftOut(_dataPin,_sckPin,MSBFIRST,Data[i]);
	_rckPin->write(HIGH);
}
