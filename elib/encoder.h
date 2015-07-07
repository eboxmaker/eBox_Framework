/*
file   : encoder.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __ENCODER_H
#define __ENCODER_H
#include "ebox.h"
#include "button.h"
class Encoder
{
	public:
		Encoder(uint8_t Apin,uint8_t Bpin);
		int ReadEncoder();


	private:
		
	uint8_t _Apin;
	uint8_t	_Bpin;
	
	uint8_t _astate;
	uint8_t _alaststate;

	uint8_t _bstate;
	uint8_t _blaststate;

};
#endif
