/*
file   : 74hc595.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __BUTTON_H
#define __BUTTON_H

#include "ebox.h" 

class _74hc595
{
    public:
        _74hc595(uint8_t dataPin, uint8_t sckPin,uint8_t rckPin);
				void rowOut(uint8_t* Data,uint8_t dataLen);
		
    
    private:
			uint8_t _dataPin;
			uint8_t _sckPin;
			uint8_t _rckPin;

		
};

#endif
