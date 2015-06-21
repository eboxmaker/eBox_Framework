#ifndef __BUTTON_H
#define __BUTTON_H

#include "ebox.h" 

class _74hc595
{
    public:
        _74hc595(uint8_t dataPin, uint8_t sckPin,uint8_t rckPin);
				void RowOut(uint8_t* Data,uint8_t dataLen);
		
    
    private:
			uint8_t _dataPin;
			uint8_t _sckPin;
			uint8_t _rckPin;

		
};

#endif
