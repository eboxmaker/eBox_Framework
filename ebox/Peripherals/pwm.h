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
	1.֧��TIM2,3,4�µ�CH1-4ͨ����
	2.�����װ��һ��analogWrite����������ֱ�ӵ���analogWriteʵ��pwm
	3.������ʱ����pwm�Ĳ����޸ĺ���������setfrq��������ı�ռ�ձ�,���ǻ�Ӱ��ͬ��ʱ��������ͨ���Ĺ�����
	ע�⣺1.��ʱû������֧��remap���š�
					���ʹ��remap������Ҫ�Լ���ִ�����ʼ����������ִ����Ӧ��remap������
				2.ʹ������Ӧ��timxΪpwm����󣬸�tim�Ͳ����ٳ�ʼ��������Ĺ���
				3.��ʼ��pwm��ֻ�е���setDuty������Ż����pwm
				4.���duty���ڵ���1000����������ߵ�ƽ
*/

	
//PWMPin:�ж�ʱ��ocͨ����io
//period����ʱ�������ڡ����ֵ
//frq = 1-720k;��72k���ڿ��Ա�֤ռ�ձȾ���Ϊ0.1%������72k��ռ�ձȾ���Ϊ1%��
//pwm��ռ�ձ�=duty/1000;��duty��0-1000��

class PWM
{	
	public:
		PWM(GPIO*  PWMPin,uint32_t Frq);
		void set_frq(uint32_t Frq);
		void set_duty(uint16_t Duty);

	private:
		TIM_TypeDef* 	TIMx;
		uint32_t 			rcc;
		uint8_t 			ch;
	
		uint16_t 			period;//�������ֵ�����ڼ���ռ�ձ�
	  uint16_t 			duty;//����ռ�ձ�ֵ
	

		GPIO* 				pwmPin;

	  void init_info(GPIO* PWMPin);
		void base_init(uint16_t Period,uint16_t Prescaler);
	
};
void analog_write(GPIO* pin, uint16_t Duty);

#endif
