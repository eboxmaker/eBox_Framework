/*
file   : exti.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __EXTI_H
#define __EXTI_H
#include "common.h"
/*
	1.�ṩһ��io�ж�
	*ע�⣺stm32һ���ж���EXTI_Linexֻ�����ӵ�һ��port��GPIO_Pin_x��������PA0Ϊ�ж�Դ֮��Ͳ�������PB0��PC0��Ϊ�ж�Դ
*/

//EXTITrigger_TypeDef����ֵ
//	EXTI_Trigger_Rising 
//	EXTI_Trigger_Falling  
//	EXTI_Trigger_Rising_Falling 
class EXTIx
{
	public:
		EXTIx(GPIO* EXTIPin,EXTITrigger_TypeDef trigger);
		void attachInterrupt(void (*callbackFun)(void));
	private:
		uint8_t PortSource;
		uint8_t PinSource;
		uint32_t ExtiLine;
		uint8_t irq;
	
		void initInfo(GPIO* EXTIPin);
};

#endif
