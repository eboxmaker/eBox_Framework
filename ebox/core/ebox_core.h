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
#include "ebox_printf.h"


#include "../core/ebox_type.h"
#include "../core/new.h"
#include "../core/print.h"
#include "../core/binary.h"
#include "../core/mem/ebox_mem.h"
#include "../core/math/random.h"
#include "../core/math/crc.h"
#include "../core/math/math_misc.h"
#include "../core/ringbuf.h"
#include "../core/list.h"
#include "../core/util.h"
#include "../core/big_little.h"
#include "math.h"


#include "../core/port/ebox_port_gpio.h"
#include "../core/port/ebox_port_spi.h"
#include "../core/port/ebox_port_i2c.h"

#define EBOX_VERSION "2.1"


/////////////////////////////////////////


#define low_byte(w) ((uint8_t) ((w) & 0xff))
#define high_byte(w) ((uint8_t) ((w) >> 8))


#define bit_read(value, bit) (((value) >> (bit)) & 0x01)
#define bit_set(value, bit) ((value) |= (1UL << (bit)))
#define bit_clear(value, bit) ((value) &= ~(1UL << (bit)))
#define bit_write(value, bit, bitvalue) (bitvalue ? bit_set(value, bit) : bit_clear(value, bit))


void        shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t     shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order);





#endif
