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
3.计数器最大值为0xffff,使用的时候需要注意防止计数器溢出。
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
*/
class IN_CAPTURE
{
    public:
        uint16_t    prescaler;//保存溢出值，用于计算占空比
        uint8_t     polarity;
    public:
        IN_CAPTURE(GPIO *capture_pin);
        void        begin(uint16_t prescaler);
        uint16_t    get_capture();
        void        set_count(uint16_t count);
        void        set_polarity_falling();
        void        set_polarity_rising();
        void        attch_interrupt(void(*callback)(void));

    private:
        GPIO        *capture_pin;
        TIM_TypeDef *TIMx;
        uint8_t     ch;
        uint16_t    period;//保存溢出值，用于计算占空比

        void        init_info(GPIO *capture_pin);
        void        base_init(uint16_t Period,uint16_t Prescaler);

        uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
        void        (*_set_polarity)(TIM_TypeDef *TIMx,uint16_t TIM_OCPolarity);//设置为下降沿捕获
};
#endif
