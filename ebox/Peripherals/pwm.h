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
	3.可以随时调用pwm的参数修改函数。调用setfrq函数不会改变占空比
	注意：1.暂时没有完美支持remap引脚。
					如果使用remap引脚需要自己在执行完初始化函数后再执行相应的remap函数。
				2.使用了相应的timx为pwm服务后，该tim就不能再初始化成另外的功能
				3.初始化pwm后，只有调用setDuty函数后才会输出pwm
				4.如果duty大于1000则输出持续高电平
*/

//默认配置参数
#define PWM_TIM_PERIOD 		1000
#define PWM_TIM_PRESCALER	72
	
//PWMPin:有定时器oc通道的io
//period：定时器的周期。溢出值
//prescaler:定时器预分频器
//pwm的频率 = 72M/72/1000;
//pwm的占空比=duty/1000;（duty：0-1000）

class PWM
{	
	public:
		PWM(GPIO*  PWMPin);
		PWM(GPIO*  PWMPin,uint16_t Period,uint16_t Prescaler);
		void setFrq(uint16_t Period,uint16_t Prescaler);
		void setFrq(uint32_t Frq);
		void setDuty(uint16_t Duty);

	private:
		TIM_TypeDef* 	TIMx;
		uint32_t 			rcc;
		uint8_t 			ch;
		uint32_t			irq;
	
		uint16_t 			period;
		uint16_t 			prescaler;
	  uint16_t 			duty;
		uint32_t 			frq;
	uint32_t 	p;
	

		GPIO* 				pwmPin;

	  void initInfo(GPIO* PWMPin);
		void baseInit(uint16_t Period,uint16_t Prescaler);
	
};
void analogWrite(GPIO* pin, uint16_t Duty);

#endif
