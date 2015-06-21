#ifndef __WIRING_ANALOG_H
#define __WIRING_ANALOG_H

#include "ebox.h"
#ifdef __cplusplus
extern "C"{
#endif
void Init_ADC1(void);
uint16_t analogRead(uint8_t pin);
uint16_t analogReadToVoltage(uint8_t pin); 
	
#ifdef __cplusplus
}
#endif

#endif
