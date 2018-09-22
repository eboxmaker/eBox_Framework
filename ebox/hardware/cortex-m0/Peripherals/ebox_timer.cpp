/**
  ******************************************************************************
  * @file    timer.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "ebox_timer.h"
#include "ebox_timer_it.h"

#if USE_PRINTF
// 是否打印调试信息, 1打印,0不打印
#define debug 0
#endif

#if debug
#include "ebox_uart.h"
extern Uart uart1;
#define DBG(...) uart1.printf(__VA_ARGS__)
#define  TIMER_DEBUG(...) DBG("[TIMER]  "),DBG(__VA_ARGS__)
#else
#define  TIMER_DEBUG(...)
#endif

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/
uint32_t GetClock(void){
	// 计算TIM时钟频率
	return LL_RCC_GetAPB1Prescaler() == 0 ? cpu.clock.pclk1 : (cpu.clock.pclk1*2);
}

/**
 *@brief    基类构造函数
 *@param    NONE
 *@retval   none
*/
T_base::T_base(TIM_TypeDef *TIMx){
	_timx = TIMx;
}

/**
 *@brief    使能TIM时钟
 *@param    NONE
 *@retval   none
*/
void T_base::_enableClock(){
	/* Enable the timer peripheral clock */
//	_tIndex = getPeriphIndex1((uint32_t)_timx,TIM_INFO);
//	TIM_INFO[_tIndex]._EnableClock(TIM_INFO[_tIndex]._rcc);
    rcc_clock_cmd((uint32_t)_timx,ENABLE);

	//	tim_irq_init(0,(&E_base::_irq_handler),this);
	/* Set counter mode */
	/* Reset value is LL_TIM_COUNTERMODE_UP */
	//LL_TIM_SetCounterMode(TIM2, LL_TIM_COUNTERMODE_UP);
}

void T_base::_enInterrupt(){
	/* Configure the NVIC to TIMx update interrupt */
//	NVIC_SetPriority(TIM_INFO[_tIndex]._irq, 0);
//	NVIC_EnableIRQ(TIM_INFO[_tIndex]._irq);
    _nvic(ENABLE,0,0);
	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(_timx);
}

void T_base::_nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority){
    nvic_dev_set_priority((uint32_t)_timx,0,0,0);
    if(enable != DISABLE)
        nvic_dev_enable((uint32_t)_timx,0);
    else
        nvic_dev_disable((uint32_t)_timx,0);
}

/**
 *@brief    设置TIM Prescaler & Period
 *@param    NONE
 *@retval   none
*/
void T_base::_setPerPsc(void){
	/* Set the pre-scaler value */
	LL_TIM_SetPrescaler(_timx, _prescaler-1);
	/* Set the auto-reload value*/
	LL_TIM_SetAutoReload(_timx, _period);
}

/**
 *@name     T_base::calculate(uint32_t frq)
 *@brief    根据给定频率，计算period,prescaler参数
 *@param    frq  要输出的频率，单位hz. 0-最大频率。
 *@retval   None
*/
void T_base::_calculate(uint32_t frq)
{
	uint32_t period  = 0;		// 周期
	uint32_t prescaler = 1;		// 预分频
	uint32_t ii = GetClock();

	for (; prescaler <= 0xffff; prescaler++)
	{
		period = ii / prescaler / frq;
		if (0xffff >= period)
		{
			TIMER_DEBUG("TIM clock : %d , Timer frq : %d period = %d , prescaler = %d  \r\n",ii,frq,period,prescaler);
			break;
		}
	}
	_period = period;
	_prescaler = prescaler;
}

/**
 *@brief    获取时钟频率
 *@param    NONE
 *@retval   当前TIM频率
*/
// uint32_t T_base::GetSourceClock(void){
	// return GetClock();
// }
/**
 *@brief    启动定时器
 *@param    NONE
 *@retval   NONE
*/
void T_base::_start(void){
	/* Enable counter */
	LL_TIM_EnableCounter(_timx);
	/* Force update generation 强制更新 */
	LL_TIM_GenerateEvent_UPDATE(_timx);
}

/**
 *@brief    设置计数模式
 *@param    uint32_t CounterMode
 *         @arg @ref LL_TIM_COUNTERMODE_UP
 *         @arg @ref LL_TIM_COUNTERMODE_DOWN
 *         @arg @ref LL_TIM_COUNTERMODE_CENTER_UP
 *         @arg @ref LL_TIM_COUNTERMODE_CENTER_DOWN
 *         @arg @ref LL_TIM_COUNTERMODE_CENTER_UP_DOWN
 *@retval   NONE
*/
void T_base::_setCountMode(uint32_t CounterMode){
	LL_TIM_SetCounterMode(_timx, CounterMode);
}

/**
 *@brief    设定TIMx工作模式
 *@param    NONE
 *@retval   NONE
*/
void T_base::_setMode(void){
	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(_timx);
	/* Configure the NVIC to handle TIM2 update interrupt */
//	NVIC_SetPriority(TIM_INFO[_tIndex]._irq, 0);
//	NVIC_EnableIRQ(TIM_INFO[_tIndex]._irq);
    _nvic(ENABLE,0,0);
}
/*********************************  E_TIME  *****************************************/
/**
 *@brief    设置定时器时间
 *@param    uint32_t us  
 *@retval   NONE
*/
void Timer::setUs(uint32_t us){
	uint32_t frq = 1000000/us;
	setFrequency(frq);
}

void Timer::setMs(uint32_t ms){
	uint32_t frq = 1000/ms;
	setFrequency(frq);
}

/**
 *@brief    设置定时器频率
 *@param    uint32_t frq  0-TIM时钟频率
 *@retval   NONE
*/
void Timer::setFrequency(uint32_t frq){
	if (frq >= GetClock())//控制频率，保证其有1%精度
		frq = GetClock();
	TIMER_DEBUG("frq %d , max frq %d \r\n",frq,GetClock());
	_calculate(frq);
	_enableClock();
	_setPerPsc();
    
    switch((uint32_t)_timx)
    {
    case (uint32_t)TIM1_BASE:
        _tIndex = TIM1_IT_Update;
        break;
    case (uint32_t)TIM2_BASE:
        _tIndex = TIM2_IT_Update;
        break;
    case (uint32_t)TIM3_BASE:
        _tIndex = TIM3_IT_Update;
        break;
//    case (uint32_t)TIM4_BASE:
//        index = TIM4_IT_Update;
//        break;
//    case (uint32_t)TIM5_BASE:
//        index = TIM5_IT_Update;
//        break;
    case (uint32_t)TIM6_BASE:
        _tIndex = TIM6_IT_Update;
        break;
    case (uint32_t)TIM7_BASE:
        _tIndex = TIM7_IT_Update;
        break;
    }	
    tim_irq_init(_tIndex,(&Timer::_irq_handler),(uint32_t)this);
}

/**
 *@brief    启动定时器
 *@param    NONE
 *@retval   NONE
*/
void Timer::start(void){
	_enInterrupt();
	_start();
}

/**
 *@brief    关闭定时器
 *@param    NONE
 *@retval   NONE
*/
void Timer::stop(void){
	LL_TIM_DisableCounter(_timx);	
}

void Timer::_setMode(void){
	/* Enable the update interrupt */
	LL_TIM_EnableIT_UPDATE(_timx);
	/* Configure the NVIC to handle TIM2 update interrupt */
    _nvic(ENABLE,0,0);
}

/**
 *@brief    获取定时器最大频率
 *@param    NONE
 *@retval   NONE
*/
uint32_t Timer::GetMaxFrequency(void){
	return GetClock();
}

void Timer::_irq_handler(uint32_t id){
	Timer *hander = (Timer*)id;
	hander->_pirq.call();
}





