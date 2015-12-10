/*
file   : rtc.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __RTC_H
#define __RTC_H
#include "common.h"
/*
	1.提供一个32位的循环计数,每秒加1.
	2.一个中断源，三个中断事件
	3.支持一个简易时钟
	4.支持一个闹钟
*/


#define RTC_CLOCK_LSI 0
#define RTC_CLOCK_LSE 1
#define RTC_CLOCK_HSI 2//不支持，暂时不可用

//中断事件源
#define RTC_EVENT_OW		((uint16_t)0x0004)  /*!< Overflow interrupt */
#define RTC_EVENT_ALR 	((uint16_t)0x0002)  /*!< Alarm interrupt */
#define RTC_EVENT_SEC		((uint16_t)0x0001)  /*!< Second interrupt */

//用户配置
#define RTC_CFG_FLAG 0XA5A5
#define RTC_CLOCK_SOURCE RTC_CLOCK_LSI
//#define RTC_CLOCK_SOURCE RTC_CLOCK_LSE

class RTC_CLASS
{
    public:
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
    public:
        void begin();
        void attach_interrupt(uint16_t event, void (*callbackFun)(void));
        void interrupt(uint32_t bits,FunctionalState x);
        void set_counter(uint32_t count);
        void set_alarm(uint32_t count);
        uint32_t get_counter();
        void set_time_HMS(uint8_t h,uint8_t m,uint8_t s);
        void set_alarm  (uint8_t h,uint8_t m,uint8_t s);
        void get_time_HMS(uint8_t *h,uint8_t *m,uint8_t *s);

    private:
        void    config();
        uint8_t is_config(uint16_t configFlag);
        void    set_config_flag(uint16_t configFlag);
};
extern RTC_CLASS 	rtc;

#endif

