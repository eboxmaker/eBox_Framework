/*
file   : button.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __Button_h
#define __Button_h

#include "common.h"

class Button
{
    public:
        Button(void);
        Button(uint8_t pin, uint8_t puEnable);
			uint8_t read(void);
			uint8_t release(void);
			uint8_t click(void);
			uint8_t pressedFor(uint32_t ms,uint8_t times);
    
    private:
        uint8_t _pin;           //arduino pin number
        uint8_t _puEnable;      //internal pullup resistor enabled
        uint8_t _state;         //current button state
        uint8_t _lastState;     //previous button state
        uint8_t _changed;       //state changed since last read
        uint8_t _longpressflag;
		uint8_t _longpresstimes;
        uint32_t _time;         //time of current state (all times are in ms)
        uint32_t _lastChange;   //time of last state change
        uint32_t _dbTime;       //debounce time
        
};
#endif
