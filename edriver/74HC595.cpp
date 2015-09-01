/*
file   : 74hc595.cpp
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
	
	_dataPin->mode(OUTPUT_PP);
	_sckPin->mode(OUTPUT_PP);
	_rckPin->mode(OUTPUT_PP);
}

void _74hc595::rowData(uint8_t* Data,uint8_t dataLen,uint8_t bitOder)
{
	_rckPin->write(LOW);
	for(int i = 0; i < dataLen; i++)
	shiftOut(_dataPin,_sckPin,bitOder,Data[i]);
}

void _74hc595::rowData(uint8_t data,uint8_t bitOder)
{
	_rckPin->write(LOW);
	shiftOut(_dataPin,_sckPin,bitOder,data);
}

void _74hc595::Out()
{
  _rckPin->write(HIGH);
}


void _74hc595::rowOut(uint8_t* Data,uint8_t dataLen,uint8_t bitOder)
{
	_rckPin->write(LOW);
	for(int i = 0; i < dataLen; i++)
	shiftOut(_dataPin,_sckPin,bitOder,Data[i]);
  _rckPin->write(HIGH);
}
void _74hc595::rowOut(uint8_t data,uint8_t bitOder)
{
	_rckPin->write(LOW);
	shiftOut(_dataPin,_sckPin,bitOder,data);
  _rckPin->write(HIGH);
}