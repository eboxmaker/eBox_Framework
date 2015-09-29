/*
file   : gtimer.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

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
	2.�ṩһ��1-720Khz�Ķ�ʱ�ж�
*/




class TIM
{
	public:
		TIM(TIM_TypeDef* TIMx);

		void begin(uint32_t Frq);
		void attach_interrupt(void(*callback)(void));
		void interrupt(FunctionalState enable);
	
		void start(void);
		void stop(void);
		void set_reload(uint16_t Autoreload);
		void clear_count(void);
		
	private:
		void base_init(uint16_t period,uint16_t prescaler);
		TIM_TypeDef *_TIMx;
	
};
#endif
