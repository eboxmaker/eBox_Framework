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
	1.提供一个1-720Khz的定时中断

*/


class TIMERONE
{
    public:
        TIMERONE();
        void begin(uint32_t frq);
        void attach_interrupt(void(*callback)(void));
        void interrupt(FunctionalState x);
        void start();
        void stop();
        void reset_frq(uint32_t frq);

    private:
        void base_init(uint16_t period,uint16_t prescaler);
        void set_reload(uint16_t autoreload);
        void clear_count(void);	
};
#endif
