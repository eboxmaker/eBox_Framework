/*
file   : encoder.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#include "encoder.h"





Encoder::Encoder(GPIO* Apin,GPIO* Bpin)
{

	_Apin = Apin;
	_Bpin = Bpin;

	_Apin->mode(INPUT_PU);
	_Bpin->mode(INPUT_PU);


}
int Encoder::ReadEncoder()
{
	_alaststate = _astate;
	_blaststate = _bstate;
	_astate = _Apin->read();
	_bstate = _Bpin->read();

	if((_astate == 0) && _alaststate && _bstate)
	{
			return 1;
	}
		if((_bstate == 0) && _blaststate && _astate)
	{
			return 2;
	}

	return 0;
}
