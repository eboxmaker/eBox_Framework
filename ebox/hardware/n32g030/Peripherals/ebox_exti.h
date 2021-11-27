/**
  ******************************************************************************
  * @file    exti.h
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

#ifndef __EXTI_H
#define __EXTI_H
#include "ebox_core.h"
#include "mcu.h"
#include "FunctionPointer.h"

/*
	1.提供一个io中断
	*注意：stm32一个中断线EXTI_Linex只能连接到一个port的GPIO_Pin_x，即设置PA0为中断源之后就不能设置PB0，PC0等为中断源
    trigger: 引脚触发中的条件，可以是一下三种模式中的一种：
        - RISING: 上升沿触发中断
        - FALLING: 下降沿触发中断
        - CHANGE: 上升沿和下降沿均触发中断
 */



class Exti
{
public:
    // 触发类型
    enum Trig_t
    {
        TrigNone = 0,	// 下降沿触发
        TrigFall,	    // 下降沿触发
        TrigRise,			// 上升沿触发
        TrigFallRise		// 上升沿下降沿
    };
    enum Mode_t
    {
        ModeIt = 0,			// 中断
        ModeEvent,			// 事件
        ModeItEvent		// 中断&事件
    };
public:
    Exti(Gpio *exti_pin);
    void begin(PinMode_t mode = INPUT, Mode_t extiMode = ModeIt);

    void nvic(FunctionalState enable, uint8_t preemption_priority = 0, uint8_t sub_priority = 0);
    void interrupt(Trig_t type, FunctionalState enable);
    void soft_triger();
    bool read(){return _pin->read();}

    /**
    *@brief    exti 绑定中断
    *@param    TrigType type 中断触发类型,FALLING,RISING,FALL_RISING; void (*fptr)(void) 回调函数指针
    *@retval   NONE
    */
    void attach(void (*fptr)(void));
    /**
     *@brief    exti 绑定成员回调函数
     *@param    TrigType type 中断触发类型,FALLING,RISING,FALL_RISING;
    	          T* tptr 对象指针
    		        void (T::*mptr)(void) 对象成员
     *@retval   NONE
    */
    template<typename T>
    void attach(T *tptr, void (T::*mptr)(void))
    {
        _pirq.attach(tptr, mptr);
    }

private:
    // 静态成员不依赖类的创建，即使不创建类，它也存在，且可以在外部通过类名访问: IRQ::irq_handler(0)
    // 所有实例共享静态成员，静态成员不能访问普通成员，需要通过对象名间接访问
    static void _irq_handler(uint32_t pObj);
    // 回调函数指针数组，分别绑定下降沿回调和上升沿回调函数
    FunctionPointer _pirq;
    Gpio	*_pin;
    uint16_t	_extiLine;	//外部中断0-15

};
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
