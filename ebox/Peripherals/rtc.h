/*
file   : rtc.h
author : shentq
version: V1.1
date   : 2016/4/6

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


//用户配置
#define RTC_CLOCK_LSI 0
#define RTC_CLOCK_LSE 1
#define RTC_CLOCK_HSI 2//不支持，暂时不可用

#define RTC_CLOCK_SOURCE RTC_CLOCK_LSI
//#define RTC_CLOCK_SOURCE RTC_CLOCK_LSE


class RTC_CLASS
{

public:
    void begin();

    void attach_overflow_interrupt(void (*cb_fun)(void));
    void attach_alarm_interrupt(void (*cb_fun)(void));
    void attach_sec_interrupt(void (*cb_fun)(void));

    void overflow_interrupt(FunctionalState state);
    void alarm_interrupt(FunctionalState state);
    void sec_interrupt(FunctionalState state);


    void set_counter(uint32_t count);
    void set_alarm(uint32_t count);
    uint32_t get_counter();


private:
    void    config();
    uint8_t is_config(uint16_t configFlag);
    void    set_config_flag(uint16_t configFlag);
    void    nvic(FunctionalState state);
};
class RTC_CLOCK
{
    public:
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
    public:
        void begin();
        void sec_event()
        {
            count ++;
            count %=3600*24;
            sec  = (count % 60);
            min  = (count % 3600)/ 60;
            hour = (count / 3600);
        }
        void set_clock(uint8_t h,uint8_t m,uint8_t s)
        {
            count = h*3600 + m*60 + s;        
        }
    private:
       uint32_t count;
};
extern RTC_CLASS 	rtc;

#endif

