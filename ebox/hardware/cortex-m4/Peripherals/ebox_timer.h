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
#ifndef __EBOX_GTIMER_H
#define __EBOX_GTIMER_H

#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

/*
    简介：使定时器产生一个定时中断。
	1.支持tim1~14
    2.定时器的时钟源频率可以通过get_timer_source_clock()获得；
    3.定时器最大中断频率可由get_max_frq()获得；
*/
class Timer
{
public:
    Timer(TIM_TypeDef *TIMx);
    void begin(uint32_t frq);

    //    void attach_interrupt(void(*callback)(void));
    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void interrupt(FunctionalState enable);

    void start(void);
    void stop(void);
    void reset_frq(uint32_t frq);

    uint32_t get_timer_source_clock();
    uint32_t get_max_frq();

    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _irq.attach(tptr, mptr);
    }
private:
    void base_init(uint32_t _period, uint32_t _prescaler);
    void set_reload(uint16_t auto_reload);
    void clear_count(void);
    TIM_TypeDef *TIMx;
protected:
    FunctionPointer _irq;
};
#endif
