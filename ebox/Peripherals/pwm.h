/*
file   : pwm.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __PWM_H
#define __PWM_H

#include "common.h"
/*
	1.֧��TIM2,3,4�µ�CH1-4ͨ����
	2.�����װ��һ��analogWrite����������ֱ�ӵ���analogWriteʵ��pwm
	3.������ʱ����pwm�Ĳ����޸ĺ���������setfrq��������ı�ռ�ձ�
	ע�⣺1.��ʱû������֧��remap���š�
					���ʹ��remap������Ҫ�Լ���ִ�����ʼ����������ִ����Ӧ��remap������
				2.ʹ������Ӧ��timxΪpwm����󣬸�tim�Ͳ����ٳ�ʼ��������Ĺ���
				3.��ʼ��pwm��ֻ�е���setDuty������Ż����pwm
				4.���duty����1000����������ߵ�ƽ
*/

//Ĭ�����ò���
#define PWM_TIM_PERIOD 		1000
#define PWM_TIM_PRESCALER	72
	
//PWMPin:�ж�ʱ��ocͨ����io
//period����ʱ�������ڡ����ֵ
//prescaler:��ʱ��Ԥ��Ƶ��
//pwm��Ƶ�� = 72M/72/1000;
//pwm��ռ�ձ�=duty/1000;��duty��0-1000��

class PWM
{	
	public:
		PWM(GPIO*  PWMPin);
		PWM(GPIO*  PWMPin,uint16_t Period,uint16_t Prescaler);
		void setFrq(uint16_t Period,uint16_t Prescaler);
		void setDuty(uint16_t Duty);

	private:
		TIM_TypeDef* 	TIMx;
		uint32_t 			rcc;
		uint8_t 			ch;
		uint32_t			irq;
	
		uint16_t 			period;
		uint16_t 			prescaler;
	  uint16_t 			duty;
	

		GPIO* 				pwmPin;

	  void initInfo(GPIO* PWMPin);
		void baseInit(uint16_t Period,uint16_t Prescaler);
	
};
void analogWrite(GPIO* pin, uint16_t Duty);

#endif
