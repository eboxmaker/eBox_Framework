/**
  ******************************************************************************
  * @file    gtimer.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __GTIMER_H
#define __GTIMER_H

#include "common.h"

/*
	1.֧��tim2,3,4
	2.�ṩһ��1-1Mhz�Ķ�ʱ�ж�
*/




class Timer
{
public:
    Timer(TIM_TypeDef *TIMx);
    void begin(uint32_t frq);
    void attach_interrupt(void(*callback)(void));
    void interrupt(FunctionalState enable);
    void start(void);
    void stop(void);
    void reset_frq(uint32_t frq);
private:
    void base_init(uint16_t period, uint16_t prescaler);
    void set_reload(uint16_t auto_reload);
    void clear_count(void);
    TIM_TypeDef *_TIMx;

};
#endif
