#ifndef __EBOX_TYPE_H
#define __EBOX_TYPE_H

#include "stm32f10x.h"

#define true 0x1
#define false 0x0

#define LSB_FIRST 0
#define MSB_FIRST 1

#define LOW     0
#define HIGH    1
#define CHANGE  2
#define FALLING 3
#define RISING  4

#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105


#define EOK         0
#define EWAIT       1
#define EBUSY       2
#define ETIMEOUT    3
#define EPARA       4
#define E2LONG      5
#define EOTHER      128

typedef union
{
    uint8_t byte[2];
    uint16_t value;
}DataU16_t; 

typedef union
{
    uint8_t byte[4];
    uint32_t value;
}DataU32_t; 

typedef union
{
    uint8_t byte[2];
    int16_t value;
}Data16_t; 

typedef union
{
    uint8_t byte[4];
    int32_t value;
}Data32_t; 

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
}DateTime_t;


#endif
