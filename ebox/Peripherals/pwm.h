/*
file   : pwm.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __PWM_H
#define __PWM_H

#include "common.h"
/*
	1.支持TIM2,3,4下的CH1-4通道。
	2.额外封装了一个analogWrite函数，可以直接调用analogWrite实现pwm
	3.可以随时调用pwm的参数修改函数。调用setfrq函数不会改变占空比,但是会影响同定时器下其他通道的工作。
	注意：1.暂时没有完美支持remap引脚。
					如果使用remap引脚需要自己在执行完初始化函数后再执行相应的remap函数。
				2.使用了相应的timx为pwm服务后，该tim就不能再初始化成另外的功能
				3.初始化pwm后，只有调用setDuty函数后才会输出pwm
				4.如果duty大于等于1000则输出持续高电平
*/


//PWMPin:有定时器oc通道的io
//period：定时器的周期。溢出值
//frq = 1-720k;在72k以内可以保证占空比精度为0.1%。超过72k后占空比精度为1%。
//pwm的占空比=duty/1000;（duty：0-1000）

class PWM
{
public:
    PWM(GPIO *pwm_pin);
    void begin(uint32_t frq, uint16_t duty);
    void set_frq(uint32_t frq);
    void set_duty(uint16_t duty);
    void set_oc_polarity(uint8_t flag);
private:
    GPIO *pwm_pin;
    TIM_TypeDef *TIMx;
    uint32_t    rcc;
    uint8_t     ch;
    uint16_t    period;//保存溢出值，用于计算占空比
    uint16_t    duty;//保存占空比值
    uint16_t    oc_polarity;

    void init_info(GPIO *pwm_pin);
    void base_init(uint16_t Period, uint16_t Prescaler);
};
void analog_write(GPIO *pwm_pin, uint16_t duty);

#endif
