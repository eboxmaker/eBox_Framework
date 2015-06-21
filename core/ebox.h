#ifndef __EBOX_H
#define __EBOX_H


#ifdef __cplusplus
 extern "C" {
#endif 

	 #include "stm32f10x.h"                  // Device header
	 #include "mcuconfig.h"

	 #include "pin_ebox.h" 
	 #include "wiring_digital.h"
	 #include "wiring_analog.h"
	 #include "interrupts.h"

	 


#define INPUT 0x0
#define INPUT_PULLUP 0x2
#define INPUT_PULLDOWN 0x3
	
#define OUTPUT 0x1
	
#define AF_PP 0x4
#define AF_OD 0x5
#define AIN 0x6





	 
 
	 
	 
	
	 
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105


#define true 0x1
#define false 0x0
 
#define HIGH 0x1
#define LOW  0x0

#define LSBFIRST 0
#define MSBFIRST 1

#define CHANGE 2
#define FALLING 3
#define RISING 4
 

//interrupts functions are in WInterrupts.c file
#define Interrupts() __enable_irq()
#define noInterrupts() __disable_irq()

//#define lowByte(w) ((uint8_t) ((w) & 0xff))
//#define highByte(w) ((uint8_t) ((w) >> 8))

//#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
//#define bitSet(value, bit) ((value) |= (1UL << (bit)))
//#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
//#define bitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

//typedef unsigned int word;

//#define bit(b) (1UL << (b))	 
	 
	 
void eBoxInit(void);	 
uint32_t millis( void ) ;

void delay_ms(uint32_t ms);
void delay(uint32_t ms );

void delay_us(uint16_t us);
void delayMicroseconds(uint16_t us);
void _delayus(uint32_t us);


void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder);



#ifdef __cplusplus
 }
#endif
	 

 #endif
 
