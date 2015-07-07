/*
file   : gtimer.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/
#ifndef __GTIMER_H
#define __GTIMER_H

#include "common.h"

/*
	1.֧��tim2,3,4
	2.�ṩһ��xhz���ж�
*/


//Ĭ������ΪԤ��Ƶ72��Ƶ
//Ĭ������Ϊ���ֵ 1000
//Ĭ�����ã�1ms�ж�һ��
#define PRESCALE 72
#define PEROID	1000


class TIM
{
	protected:
		

	
	public:


		TIM(TIM_TypeDef* TIMx);
		TIM(TIM_TypeDef* TIMx,uint32_t period,uint32_t prescaler);
	
		void begin(void);
		void attachInterrupt(void(*callback)(void));
		void interrupt(FunctionalState x);
		void start(void);
	
		void stop(void);
		void setReload(uint16_t Autoreload);
		void clearCount(void);
		
	private:

		uint16_t _period;
		uint16_t _prescaler;
	
		uint8_t _id;
		TIM_TypeDef *_TIMx;
		uint32_t _rcc;
		uint32_t _irq;
	
		void baseInit(uint16_t period,uint16_t prescaler);
};
#endif
