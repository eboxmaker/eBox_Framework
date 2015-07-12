/*
file   : led.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __LED_H
#define __LED_H
#include "ebox.h"
#include "74hc595.h"

class LED:public _74hc595
{

	public:
		LED(GPIO* dataPin, GPIO* sckPin,GPIO* rckPin):_74hc595(dataPin,sckPin,rckPin)
		{
			
		};
		void ShowTimeHMS(uint8_t hour,uint8_t min,uint8_t sec);
		void ShowDateTime(uint16_t year,uint8_t month,uint8_t date);
	private:
		uint8_t _dataPin;
		uint8_t _sckPin;
		uint8_t _rckPin;
		uint8_t _LEDBuf[8];

};
#endif
