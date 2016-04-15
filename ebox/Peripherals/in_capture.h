/*
file   : in_capture.cpp
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __IN_CAPTURE_H
#define __IN_CAPTURE_H
#include "common.h"

#define TIM_ICPOLARITY_FALLING TIM_ICPolarity_Falling
#define TIM_ICPOLARITY_RISING  TIM_ICPolarity_Rising
/*
1.支持TIM2，3，4的ch1,2,3,4.共计12个通道
2.支持测量周期、频率、高级用法支持测量占空比
3.定时器计数器最大值为0xffff,为了突破这个限制，
    在本例程中，使用了update溢出中断调用tx_overflow_times可以将计数器
    拓展至2^32。大大提高测量范围，可以实现最高频率（1分频）测量周期低于120s的信号。
    如果使用2分频，可测量周期低于240s的信号。以此类推。
4.关于分频系数和脉冲宽度测量的计算关系，要遵循一个原则：在不溢出的情况下尽量使用低分频系数（高TIM时钟）去检测对象
5.关于get_capture()和测量时间结果转换的关系；
    时间(us)=get_capture()/(72/prescaler);
    时间(ms)=get_capture()/(72000/prescaler);
    时间(s)=get_capture()/(72000000/prescaler);
6.如果使用某个定时器通道用于输入捕获，则该定时器所有通道都必须是输入捕获模式，不能再设置为其他工作模式
重点：
    在采用低分频系数的时候，可以保证测量精度，但是会增大定时器溢出频率，进而增大cpu开销，
    在采用高分频系数的时候，测量精度较低，但是会降低定时器溢出频率，进而降低cpu开销，
    stm32在72M主频下，最高可测160Khz的信号。如果再大，将无法测量。
*/

class IN_CAPTURE
{
public:
    IN_CAPTURE(GPIO *capture_pin);
    void        begin();//使用默认参数，分频系数为1；最大量程为120s
    void        begin(uint16_t prescaler);
    void        set_count(uint16_t count);
    void        set_polarity_falling();
    void        set_polarity_rising();
    uint32_t    get_capture();
    uint32_t    get_zone_time_us();
    void        attch_ic_interrupt(void(*callback)(void));
    void        attch_update_interrupt(void(*callback)(void));

    uint8_t     polarity;
private:
    GPIO        *capture_pin;
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    period;//保存溢出值，用于计算占空比
    uint16_t    prescaler;//保存溢出值，用于计算占空比

    uint16_t    *overflow_times;
    uint32_t    last_value;

    void        init_info(GPIO *capture_pin);
    void        base_init(uint16_t Period, uint16_t Prescaler);

    uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
    void        (*_set_polarity)(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity); //设置为下降沿或者上升沿捕获
};
#endif
