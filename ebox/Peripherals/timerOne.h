/*
file   : timerone.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __TIMERONE_H
#define __TIMERONE_H
#include "common.h"

/*
	1.�ṩһ��1-720Khz�Ķ�ʱ�ж�

*/


class TIMERONE
{
	public:
		TIMERONE();
		void begin(uint32_t Frq);
		void attach_interrupt(void(*callback)(void));
		void interrupt(FunctionalState x);
	
		void start();
		void stop();
		void set_reload(uint16_t Autoreload);
		void clear_count(void);
	
	private:
		void base_init(uint16_t period,uint16_t prescaler);
};
#endif
