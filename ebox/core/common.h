/*
file   : common.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __COMMON_H
#define __COMMON_H
#ifdef __cplusplus
extern "C"{
#endif
	
#include "core.h"                  // Device header



////////宏定义/////////////////////////////////


#define true 0x1
#define false 0x0
 
#define HIGH 0x1
#define LOW  0x0


#define LSB_FIRST 0
#define MSB_FIRST 1

#define PI 3.1415926535898


typedef enum
{ 
    AIN         = 0x0,
    INPUT       = 0x04,
    INPUT_PD    = 0x28,
    INPUT_PU    = 0x48,
    OUTPUT_OD   = 0x14,
    OUTPUT_PP   = 0x10,
    AF_OD       = 0x1C,
    AF_PP       = 0x18
}PIN_MODE;

class GPIO
{
    public:
        GPIO(GPIO_TypeDef *port, uint16_t pin);
        void mode(PIN_MODE mode);
        void set();
        void reset();
        void write(uint8_t val);
        void toggle();	
        void read(uint8_t *val);
        uint8_t read(void);
        //操作符重载
        GPIO& operator= (int value) {
            write(value);
            return *this;
        }
        operator int() {
            return read();
        }
                        
        GPIO_TypeDef* port;
        uint16_t pin;	
};
class SYSTEM
{
    public:
        uint32_t chip_id[3];
        uint16_t flash_size;
        
        void  get_chip_info();
        float get_cpu_temperature();
};
extern SYSTEM sys;

void        ADC1_init(void);

uint16_t	analog_read(GPIO *pin);	
uint16_t	analog_read_voltage(GPIO *pin); 

void        shift_out(GPIO *data_pin, GPIO *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t		shift_in(GPIO *data_pin, GPIO *clock_pin, uint8_t bit_order);




#ifdef __cplusplus
}
#endif

#endif
