
#ifndef _WIRING_DIGITAL_H
#define _WIRING_DIGITAL_H
#include "ebox.h"

#ifdef __cplusplus
 extern "C" {
#endif
	 
void pinMode(uint32_t pin, uint32_t mode);
void digitalWrite( uint32_t Pin, uint32_t ulVal );
int digitalRead( uint32_t pin );


#ifdef __cplusplus
}
#endif
#endif
