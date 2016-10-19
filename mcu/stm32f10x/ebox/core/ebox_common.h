/**
  ******************************************************************************
  * @file    common.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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
 *      *分离了GPIO、analog和common的耦合关系
 *      *增加了date_time_t类型
 *      *删除了sys的，将其移至core
 *      *删除了parallel_gpio的，将其移至edriver
 */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __COMMON_H
#define __COMMON_H


#include "ebox_core.h" 
#include "ebox_gpio.h"
#include "ebox_analog.h"
#include "ebox_mem.h"

#define EBOX_VERSION "2.0"

#define EOK         0
#define EWAIT       1
#define EBUSY       2
#define ETIMEOUT    3
#define EPARA       4
#define E2LONG      5
#define EOTHER      128

////////宏定义/////////////////////////////////


#define true 0x1
#define false 0x0

#define HIGH 0x1
#define LOW  0x0


#define LSB_FIRST 0
#define MSB_FIRST 1

#define PI 3.1415926535898

typedef struct
{
	uint8_t year;
	uint8_t month;
	uint8_t date;
	uint8_t hour;
	uint8_t min;
	uint8_t sec;
	uint8_t week;
}date_time_t;


void        random_seed(uint16_t seed);
uint16_t    random();
uint16_t    random(uint16_t max);
uint16_t    random(uint16_t min, uint16_t max);

void        shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t     shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order);


#endif
