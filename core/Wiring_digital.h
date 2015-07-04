
#ifndef _WIRING_DIGITAL_H
#define _WIRING_DIGITAL_H
#include "ebox.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
void pinMode(uint32_t pin, uint32_t mode);
void digitalWrite( uint32_t pin, uint32_t ulVal );
int digitalRead( uint32_t pin );

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);


#ifdef __cplusplus
}
#endif
#endif
