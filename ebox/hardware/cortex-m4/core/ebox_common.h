/*
file   : common.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __EBOX_COMMON_H
#define __EBOX_COMMON_H

#include "mcu_core.h"     
#include "../core/ebox_core_.h"
#include "ebox_gpio.h"



void        ADC1_init(void);

uint16_t	analog_read(Gpio *pin);
uint16_t	analog_read_voltage(Gpio *pin);



void        shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t     shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order);


#endif
