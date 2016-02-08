/*
file   : in_capture.cpp
author : shentq
version: V1.0
date   : 2015/7/5

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
    在本例程中，如果使用了update溢出中断调用overflow_event_process可以将计数器
    拓展至2^32。大大提高测量范围，可以实现最高频率（1分频）测量周期低于60s的信号，如果信号长度。
    如果使用2分频，可测量周期低于120s的信号，如果信号长度。以此类推。
4.get_captur()的精度补偿值
    //此处要加一个简单的修正值,此数为测试大致测试结果
    //分频系数 | 补偿值 | TIM时钟频率 | 测试频率
    //7200分频 | 0      | 0.01M       |  1hz-100hz
    //720分频  | 1      | 0.10M       |  100-1K
    //72分频   | 2      | 1.00M       |  1K
    //64分频   | 3      | 1.125M      |  1K
    //36分频   | 5      | 2.00M       |  1K
    //32分频   | 6      | 2.25M       |  1K
    //18分频   | 11     | 4.00M       |  1K
    //16分频   | 11     | 4.50M       |  1K
    //9分频    | 18     | 8.00M       |  10K
    //8分频    | 21     | 9.00M       |  10K-50K
    //4分频    | 42     | 18.0M       |  10K-50K
    //2分频    | 84     | 36.0M       |  10K-50K
    //1分频    | 168    | 72.0M       |  10K-50K
5.关于分频系数和脉冲宽度测量的计算关系，要遵循一个原则：在不溢出的情况下尽量使用低分频系数（高TIM时钟）去检测对象
重点：
    在采用低分频系数的时候，可以保证测量精度，但是会增大定时器溢出频率，进而增大cpu开销，
    在采用高分频系数的时候，测量精度较低，但是会降低定时器溢出频率，进而降低cpu开销，
    stm32在72M主频下，最高可测160Khz的信号。如果再大，将无法测量。
*/
typedef enum {
    IC_NONE = 0,
    IC_OVERFLOW = 1,
    IC_FAULT = 2
}IC_OVERFLOW_STATE_TYEP;

class IN_CAPTURE
{
    public:
        IN_CAPTURE(GPIO *capture_pin);
        void        begin(uint16_t prescaler);
        void        set_count(uint16_t count);
        void        set_polarity_falling();
        void        set_polarity_rising();
        void        overflow_event_process();
        uint32_t    get_capture();
        IC_OVERFLOW_STATE_TYEP        get_overflow_state();
        void        attch_ic_interrupt(void(*callback)(void));
        void        attch_update_interrupt(void(*callback)(void));

    private:
        GPIO        *capture_pin;
        TIM_TypeDef *TIMx;
        uint8_t     ch;
        uint16_t    period;//保存溢出值，用于计算占空比
        uint16_t    prescaler;//保存溢出值，用于计算占空比
        uint16_t    overflow_times;//溢出次数
        IC_OVERFLOW_STATE_TYEP     overflow_state;//溢出的情况。如果发生16位溢出：IC_OVERFLOW，如果发送32位溢出:IC_FAULT
        uint8_t     polarity;
    
        void        init_info(GPIO *capture_pin);
        void        base_init(uint16_t Period,uint16_t Prescaler);

        uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
        void        (*_set_polarity)(TIM_TypeDef *TIMx,uint16_t TIM_OCPolarity);//设置为下降沿捕获
};
#endif
