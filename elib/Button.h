/*----------------------------------------------------------------------*
 * ebox Button Library v1.0                                          *
 * shentqlf 
 *                                                                      *
 
 *----------------------------------------------------------------------*/
#ifndef __Button_h
#define __Button_h

#include "ebox.h"

class Button
{
    public:
        Button(void);
        Button(uint8_t pin, uint8_t puEnable);
			uint8_t read(void);
			uint8_t Release(void);
			uint8_t Click(void);
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
