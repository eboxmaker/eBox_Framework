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

#include "ebox_common.h"
#include "FunctionPointer.h"
/*
	1.支持tim1,2,3,4,5,6,7
    2.定时器的时钟源频率可以通过get_timer_clock()获得；
    3.定时器最大中断频率可由get_max_frq()获得；
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *增加了TIM1的支持；
 *      *增加了get_timer_clock(),get_max_frq()两个函数接口
 *      *修复了芯片主频变动导致定时器中断频率变化的bug，支持任意主频下正常工作
 */

class Timer
{
public:
    Timer(TIM_TypeDef *TIMx);
    void begin(uint32_t frq);
    void interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void start(void);
    void stop(void);
    void reset_frq(uint32_t frq);

    uint32_t get_timer_source_clock();
    uint32_t get_max_frq();

    static void _irq_handler( uint32_t id);
    void attach(void (*fptr)(void));
    template<typename T>
    void attach(T* tptr, void (T::*mptr)(void)) {
        _irq.attach(tptr, mptr);
    }
private:
    void base_init(uint16_t period, uint16_t prescaler);
    void set_reload(uint16_t auto_reload);
    void clear_count(void);
    TIM_TypeDef *_TIMx;
    uint8_t nvic_ch;
protected:
    FunctionPointer _irq;
};
#endif
