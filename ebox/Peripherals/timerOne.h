/*
file   : timerone.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentqlf@163.com. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentqlf@163.com.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentqlf@163.com assumes no responsibility for any errors contained herein.
*/

#ifndef __TIMERONE_H
#define __TIMERONE_H
#include "common.h"

/*
	1.�ṩһ��xhz���ж�

*/

//Ĭ������ΪԤ��Ƶ72��Ƶ
//Ĭ������Ϊ���ֵ 1000
//Ĭ�����ã�1ms�ж�һ��
#define PRESCALE 72
#define PEROID	1000

class TIMERONE
{

	private:
		uint16_t _period;
		uint16_t _prescaler;
		void baseInit(uint16_t period,uint16_t prescaler);
	

	public:
		TIMERONE();
		void begin();
		void start();
		void attachInterrupt(void(*callback)(void));
		void interrupt(FunctionalState x);
	
		void stop();
		void setReload(uint16_t Autoreload);
		void clearCount(void);
};
#endif