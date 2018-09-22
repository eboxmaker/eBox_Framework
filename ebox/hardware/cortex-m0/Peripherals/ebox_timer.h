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
typedef void (*pfun)(TIM_TypeDef *,uint32_t);

typedef struct{
	PIN_ID_t	_pin_id;		//pin_id
	PIN_MODE	_pin_date;	    //pin 参数， mode，outputtyper,updown
	uint8_t		_pin_af;		//af功能
	uint32_t	_periph_OR_ch;	//外设名或通道号
}AF_FUN_S;

static const AF_FUN_S TIM_MAP[] ={
	// TIM3,CH1,CH2,CH4
	PA6_ID,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH1,
	PA7_ID,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH2,
	PB1_ID,AF_PP,LL_GPIO_AF_1,TIM3_BASE+TIMxCH4,
	// TIM16,CH1
	PA6_ID,AF_PP,LL_GPIO_AF_5,TIM16_BASE+TIMxCH1,
	// TIM17,CH1
	PA7_ID,AF_PP,LL_GPIO_AF_5,TIM17_BASE+TIMxCH1,
	// TIM14,CH1
	PA4_ID,AF_PP,LL_GPIO_AF_4,TIM14_BASE+TIMxCH1,
	// TIM1,CH2,CH3
	PA8_ID,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH1,
	PA9_ID,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH2,
	PA10_ID,AF_PP,LL_GPIO_AF_2,TIM1_BASE+TIMxCH3,
	(PIN_ID_t)0xff
};

/**
 *@brief    根据Pin_id获取对应外设索引
 *@param    val：1：输出高电平；0：输出低电平
 *@retval   NONE
*/
__STATIC_INLINE uint8_t getIndex(PIN_ID_t pin_id,uint32_t periph,const AF_FUN_S *emap)
{
	uint8_t i = 0;
	while (!((0xffffff00 & (emap+i)->_periph_OR_ch) == periph) || !((emap+i)->_pin_id == pin_id))
	{
		if ((emap+i)->_pin_id == 0xff){
			return (uint8_t)0xff;
		}
		i++;
	}
	return i;
}


class T_base{
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

class Timer:T_base{
public:
	Timer(TIM_TypeDef *TIMx):T_base(TIMx){};
	// 设置time周期,us
	void setUs(uint32_t us);
	void setMs(uint32_t ms);
	// 设置time频率,hz
	void setFrequency(uint32_t frq);
	void start();
	void stop();

	
	uint32_t GetMaxFrequency(void);
        
    // 保持接口一致    
    void begin(uint32_t frq){setFrequency(frq);};
    uint32_t get_timer_source_clock(){return GetMaxFrequency();};
    uint32_t get_max_frq(){return GetMaxFrequency()/400;};
    void interrupt(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0){};
		
	void attach(void (*fptr)(void)){
		_pirq.attach(fptr);
	}
	template<typename T>
	void attach(T* tptr, void (T::*mptr)(void)){
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
