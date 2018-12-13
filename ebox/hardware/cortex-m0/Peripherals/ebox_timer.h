/**
  ******************************************************************************
  * @file    gtimer.h
  * @author  shentq
  * @version V2.1
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

#include "ebox_core.h"
#include "mcu.h"
//#include "stm32f072_define.h"
#include "mcu_define.h"
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
extern uint32_t GetClock(void);

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

// 函数指针,指向LL_TIM_OC_SetCompareCH4(TIM_TypeDef *TIMx, uint32_t CompareValue) 函数
typedef void (*pfun)(TIM_TypeDef *, uint32_t);

class T_base
{
public:
    T_base(TIM_TypeDef *TIMx);
protected:
    TIM_TypeDef  *_timx;		// TIM外设
    uint8_t		 _tIndex;		// TIM索引
    uint32_t 	 _period;		// 周期 0-65535
    uint32_t	 _prescaler;	// TIM预分频值,赋值范围1-65536,程序最终会转为0-65535

    void _calculate(uint32_t frq);
    void _setCountMode(uint32_t CounterMode);
    void _enableClock();
    void _start();
    void _setPerPsc();
    void _setMode(void);
    void _enInterrupt();		// 使能中断
    void _nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority);
};

class Timer: T_base
{
public:
    Timer(TIM_TypeDef *TIMx): T_base(TIMx) {};
    // 设置time周期,us
    void setUs(uint32_t us);
    void setMs(uint32_t ms);
    // 设置time频率,hz
    void setFrequency(uint32_t frq);
    void start();
    void stop();


    uint32_t GetMaxFrequency(void);

    // 保持接口一致
    void begin(uint32_t frq)
    {
        setFrequency(frq);
    };
    uint32_t get_timer_source_clock()
    {
        return GetMaxFrequency();
    };
    uint32_t get_max_frq()
    {
        return GetMaxFrequency() / 400;
    };
    void interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0) {};

    void attach(void (*fptr)(void))
    {
        _pirq.attach(fptr);
    }
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _pirq.attach(tptr, mptr);
    }
private:
    FunctionPointer _pirq;
    void _setMode(void);
    static void _irq_handler(uint32_t id);
};

//class Timer
//{
//public:
//    Timer(TIM_TypeDef *TIMx);
//    void begin(uint32_t frq);
//    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
//    void interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
//    void start(void);
//    void stop(void);
//    void reset_frq(uint32_t frq);

//    uint32_t get_timer_source_clock();
//    uint32_t get_max_frq();

//    static void _irq_handler( uint32_t id);
//    void attach(void (*fptr)(void));
//    template<typename T>
//    void attach(T* tptr, void (T::*mptr)(void)) {
//        _irq.attach(tptr, mptr);
//    }
//private:
//    void base_init(uint16_t period, uint16_t prescaler);
//    void set_reload(uint16_t auto_reload);
//    void clear_count(void);
//    TIM_TypeDef *_TIMx;
//    uint8_t nvic_ch;
//protected:
//    FunctionPointer _irq;
//};
#endif
