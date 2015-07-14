/*
file   : ebox.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No GPArt of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/
#ifndef __EBOX_H
#define __EBOX_H

#include "stm32f10x.h"                  // Device header


	 
#include "common.h"

#define PA0 (GPIO*)&GPA0
#define PA1 (GPIO*)&GPA1
#define PA2 (GPIO*)&GPA2
#define PA3 (GPIO*)&GPA3
#define PA4 (GPIO*)&GPA4
#define PA5 (GPIO*)&GPA5
#define PA6 (GPIO*)&GPA6
#define PA7 (GPIO*)&GPA7
#define PA8 (GPIO*)&GPA8
#define PA9 (GPIO*)&GPA9
#define PA10 (GPIO*)&GPA10
#define PA11 (GPIO*)&GPA11
#define PA12 (GPIO*)&GPA12
#define PA13 (GPIO*)&GPA13
#define PA14 (GPIO*)&GPA14
#define PA15 (GPIO*)&GPA15

#define PB0 (GPIO*)&GPB0
#define PB1 (GPIO*)&GPB1
#define PB2 (GPIO*)&GPB2
#define PB3 (GPIO*)&GPB3
#define PB4 (GPIO*)&GPB4
#define PB5 (GPIO*)&GPB5
#define PB6 (GPIO*)&GPB6
#define PB7 (GPIO*)&GPB7
#define PB8 (GPIO*)&GPB8
#define PB9 (GPIO*)&GPB9
#define PB10 (GPIO*)&GPB10
#define PB11 (GPIO*)&GPB11
#define PB12 (GPIO*)&GPB12
#define PB13 (GPIO*)&GPB13
#define PB14 (GPIO*)&GPB14
#define PB15 (GPIO*)&GPB15

#define PE5 (GPIO*)&GPE5



extern GPIO GPA0;
extern GPIO GPA1;
extern GPIO GPA2;
extern GPIO GPA3;
extern GPIO GPA4;
extern GPIO GPA5;
extern GPIO GPA6;
extern GPIO GPA7;
extern GPIO GPA8;
extern GPIO GPA9;
extern GPIO GPA10;
extern GPIO GPA11;
extern GPIO GPA12;
extern GPIO GPA13;
extern GPIO GPA14;
extern GPIO GPA15;

extern GPIO GPB0;
extern GPIO GPB1;
extern GPIO GPB2;
extern GPIO GPB3;
extern GPIO GPB4;
extern GPIO GPB5;
extern GPIO GPB5;
extern GPIO GPB6;
extern GPIO GPB7;
extern GPIO GPB8;
extern GPIO GPB9;
extern GPIO GPB10;
extern GPIO GPB11;
extern GPIO GPB12;
extern GPIO GPB13;
extern GPIO GPB14;
extern GPIO GPB15;

extern GPIO GPE5;




#include "exti.h"
#include "gtimer.h"
#include "pwm.h"
#include "i2c.h"
#include "softi2c.h"
#include "spi.h"
#include "softspi.h"
#include "uartx.h"
#include "timerone.h"
#include "rtc.h"













 #endif
 
