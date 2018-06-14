/**
  ******************************************************************************
  * @file    core.h
  * @author  shentq
  * @version V2.1
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


#include "mcu_config.h"    

#include "../core/ebox_type.h"
#include "../core/stdFun/new.h"
#include "../core/stdFun/print.h"
#include "../core/stdFun/binary.h"
#include "../core/stdFun/math_misc.h"
#include "../core/stdFun/ringbuf.h"
#include "../core/stdFun/list.h"
#include "../core/stdFun/util.h"
#include "../core/stdFun/big_little.h"
#include "../core/stdFun/random.h"
#include "../core/stdFun/crc.h"
#include "../core/stdFun/ebox_mem.h"
#include "../core/stdFun/ebox_printf.h"


#include "math.h"


#include "../core/port/ebox_port_gpio.h"
#include "../core/port/ebox_port_spi.h"
#include "../core/port/ebox_port_i2c.h"


#define EBOX_VERSION "2.1"



extern void        (*_ebox_init)(void);
extern void        (*ebox_reset)();
extern uint64_t    (*micros)();
extern uint64_t    (*millis)();
extern void        (*delay_ms)(uint64_t ms);
extern void        (*delay_us)(uint64_t ms);


extern void         get_sys_clock(cpu_t *info);
float               sys_version();


extern cpu_t cpu;

/////////////////////////////////////////


#define low_byte(w) ((uint8_t) ((w) & 0xff))
#define high_byte(w) ((uint8_t) ((w) >> 8))


/** read bit of value */
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)

/** set bit to 1 of value */
#define bit_set(value, bit) ((value) |= (1UL << (bit)))

/** set bit to 0 of value */
#define bit_clear(value, bit) ((value) &= ~(1UL << (bit)))

/** write bit of value*/
#define bit_write(value, bit, bitvalue) (bitvalue ? bit_set(value, bit) : bit_clear(value, bit))

/** Bits m to n of x */
#define bits_get(x, m, n) ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))

/** 1UL shifted left by 'shift' */
#define bit_shift(shift)                     (1UL << (shift))

/** 'Mask' shifted left by 'shift' */
#define bit_mask_shift(mask, shift)    ((mask) << (shift))

/** True iff v is a power of two (1, 2, 4, 8, ...) */
#define is_power_of_tow(v)  ((v) && !((v) & ((v) - 1)))

void        shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t     shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order);





#endif
