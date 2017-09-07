/**
  ******************************************************************************
  * @file    core.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   此文件为ebox核心文件,提供了诸多非常重要的接口,可以被标准的c文件调用。
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *修复了芯片主频只能限制为72Mhz的限制，支持任意主频
 *      *增加了cpu的信息获取，包括各个总线时钟，芯片容量，芯片计算加加的能力
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __EBOX_CORE_H
#define __EBOX_CORE_H

    
#include "../core/new.h"
#include "../core/print.h"
#include "../core/binary.h"
#include "../core/mem/ebox_mem.h"
#include "../core/math/random.h"
#include "../core/math/crc.h"
#include "../core/math/math_misc.h"
#include "../core/math/LinearRegression.h"
#include "../core/ringbuf.h"
#include "../core/list.h"
#include "../core/util.h"
#include "../core/big_little.h"
#include "math.h"


#define EBOX_VERSION "2.0"

#define EOK         0
#define EWAIT       1
#define EBUSY       2
#define ETIMEOUT    3
#define EPARA       4
#define E2LONG      5
#define EOTHER      128

/////////////////////////////////////////


#define low_byte(w) ((uint8_t) ((w) & 0xff))
#define high_byte(w) ((uint8_t) ((w) >> 8))


#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_set(value, bit) ((value) |= (1UL << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit_write(value, bit, bitvalue) (bitvalue ? bit_set(value, bit) : bit_clear(value, bit))

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

//spi相关公共宏和声明//////////////////////////////////
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV2     0
#define SPI_CLOCK_DIV4     1
#define SPI_CLOCK_DIV8     2
#define SPI_CLOCK_DIV16    3
#define SPI_CLOCK_DIV32    4
#define SPI_CLOCK_DIV64    5
#define SPI_CLOCK_DIV128   6
#define SPI_CLOCK_DIV256   7


typedef struct
{
    uint8_t     dev_num;
    uint8_t     mode;
    uint8_t     prescaler;
    uint8_t     bit_order;
} SpiConfig_t;
#endif
