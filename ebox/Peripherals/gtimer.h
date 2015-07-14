/*
file   : gtimer.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __GTIMER_H
#define __GTIMER_H

#include "common.h"

/*
	1.֧��tim2,3,4 (5,6,7��Ҫ����)
	2.�ṩһ��xhz���ж�
*/


//Ĭ������ΪԤ��Ƶ72��Ƶ
//Ĭ������Ϊ���ֵ 1000
//Ĭ�����ã�1ms�ж�һ��
#define PRESCALE 72
#define PEROID	1000


class TIM
{
	public:
		TIM(TIM_TypeDef* TIMx);

		void begin(uint32_t period,uint32_t prescaler);
		void config(uint16_t period,uint16_t prescaler);
		void attachInterrupt(void(*callback)(void));
		void interrupt(FunctionalState enable);
	
		void start(void);
		void stop(void);
		void setReload(uint16_t Autoreload);
		void clearCount(void);
		
	private:
		TIM_TypeDef *_TIMx;
	
};
#endif
