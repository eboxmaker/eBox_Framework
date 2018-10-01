/**
  ******************************************************************************
  * @file    exti.cpp
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
#include "ebox_exti.h"

// pin.id 为8bit，高4位标识port 0-5,低4位为io,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ( a & 0x0f )
/** @defgroup exti全局变量
  * @{
  */

// uint32_t pObj 类对象 ,uint8_t line 外部中断线
typedef void (*exti_irq_handler)(uint32_t pObj,uint8_t line);

static exti_irq_handler  exti_handler;   // 声明函数指针变量，指向类的静态成员
static uint32_t  exti_irq_ids[16];    	 // 保存对象地址，供静态成员识别对象，并访问对象的普通成员

void exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
{
  exti_irq_ids[index] = id;      // 保存对象地址
  exti_handler =  handler;        // 指向回调函数
}

const uint32_t EXTISorce_MAP[]={
  LL_SYSCFG_EXTI_LINE0,LL_SYSCFG_EXTI_LINE1,LL_SYSCFG_EXTI_LINE2,LL_SYSCFG_EXTI_LINE3,
  LL_SYSCFG_EXTI_LINE4,LL_SYSCFG_EXTI_LINE5,LL_SYSCFG_EXTI_LINE6,LL_SYSCFG_EXTI_LINE7,
  LL_SYSCFG_EXTI_LINE8,LL_SYSCFG_EXTI_LINE9,LL_SYSCFG_EXTI_LINE10,LL_SYSCFG_EXTI_LINE11,
  LL_SYSCFG_EXTI_LINE12,LL_SYSCFG_EXTI_LINE13,LL_SYSCFG_EXTI_LINE14,LL_SYSCFG_EXTI_LINE15
};

/**
  * @}
  */



/**
 * @brief    Exti构造函数，实例化一个对象
 * @param    *pin: 指定的外部中断引脚PA0~PG15
 * @param    trigger: 引脚触发中的条件，可以是一下三种模式中的一种：
 *           - RISING: 上升沿触发中断
 *           - FALLING: 下降沿触发中断
 *           - CHANGE: 上升沿和下降沿均触发中断
 * @return   NONE
 */
Exti::Exti(Gpio *pin)
{
  _pin = pin;
  _line = GETEXTILINE(_pin->id);
  exti_irq_init(GETEXTILINE(_pin->id),(&Exti::_irq_handler),(uint32_t)this);
}

void Exti::begin(PIN_MODE mode,ExtiType type){
  _pin->mode(mode);

  switch (type)
  {
  case IT:
    LL_EXTI_EnableIT_0_31(1<<_line);
    LL_EXTI_DisableEvent_0_31(1<<_line);
    break;
  case EVENT:
    LL_EXTI_EnableEvent_0_31(1<<_line);
    LL_EXTI_DisableIT_0_31(1<<_line);
    break;
  case IT_EVENT:
    LL_EXTI_EnableIT_0_31(1<<_line);
    LL_EXTI_EnableEvent_0_31(1<<_line);
    break;
  default:
    break;
  }
}

/**
 * @brief   初始化外部中断引脚的配置信息。
 * @param   NONE
 *
 * @return  NONE
 * @note    初始化会默认开启中断，如果用户想禁用中断，
 *          可以调用interrupt(DISABLE)关闭中断。
 */
//void Exti::begin()
//{
//  uint8_t pinN = GETEXTILINE(pin->id);
//  exti_line = 1 << pinN;
//  exti_irq_init(pinN,(&Exti::_irq_handler),(uint32_t)this);

////  pin->mode(INPUT);
//  nvic(ENABLE,0,0);
//}

//void Exti::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
//{
//  nvic_dev_set_priority((uint32_t)exti_line,0,0,0);
//  if (enable != DISABLE)
//    nvic_dev_enable((uint32_t)exti_line,0);
//  else
//    nvic_dev_disable((uint32_t)exti_line,0);
//}

///**
// * @brief   外部中断引脚的中断允许、禁止控制函数
// * @param   enable: 允许或者禁止中断
// *          - ENABLE: 允许该外部中断
// *          - DISABLE: 禁止该外部中断
// *
// * @return  NONE
// */
//void Exti::interrupt(FunctionalState enable)
//{
//  LL_EXTI_InitTypeDef EXTI_initS;
//  // connect exti line
//  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
//  LL_SYSCFG_SetEXTISource(GETEXTIPORT(pin->id), EXTISorce_MAP[GETEXTILINE(pin->id)]);

//  switch (trigger)
//  {
//  case RISING:
//    EXTI_initS.Trigger = LL_EXTI_TRIGGER_RISING;               //上升沿沿中断
//    break;
//  case FALLING:
//    EXTI_initS.Trigger = LL_EXTI_TRIGGER_FALLING ;             //下降沿都中断
//    break;
//  case CHANGE:
//    EXTI_initS.Trigger = LL_EXTI_TRIGGER_RISING_FALLING ;      //上升，下降沿都中断
//    break;
//  }
//  EXTI_initS.Line_0_31 = exti_line;
//  EXTI_initS.Mode = LL_EXTI_MODE_IT;
//  EXTI_initS.LineCommand = enable;
//  LL_EXTI_Init(&EXTI_initS);
//}

//void Exti::_init(ExtiType type){

//}
/**
 *@brief    使能中断
 *@param    NONE
 *@retval   NONE
*/
void Exti::enable(TrigType type,uint32_t priority){
  switch (type)
  {
  case FALL:
    LL_EXTI_EnableFallingTrig_0_31(1<<_line);
    LL_EXTI_DisableRisingTrig_0_31(1<<_line);
    break;
  case RISE:
    LL_EXTI_EnableRisingTrig_0_31(1<<_line);
    LL_EXTI_DisableFallingTrig_0_31(1<<_line);
    break;
  case FALL_RISING:
    LL_EXTI_EnableFallingTrig_0_31(1<<_line);
    LL_EXTI_EnableRisingTrig_0_31(1<<_line);
    break;
  default:
    break;
  }

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_SYSCFG_SetEXTISource(GETEXTIPORT(_pin->id), EXTISorce_MAP[_line]);

  switch (_line)
  {
  case 0:
  case 1:
    NVIC_EnableIRQ(EXTI0_1_IRQn);
    NVIC_SetPriority(EXTI0_1_IRQn,priority);
    break;
  case 2:
  case 3:
    NVIC_EnableIRQ(EXTI2_3_IRQn);
    NVIC_SetPriority(EXTI2_3_IRQn,priority);
    break;
  default:
    NVIC_EnableIRQ(EXTI4_15_IRQn);
    NVIC_SetPriority(EXTI4_15_IRQn,priority);
    break;
  }
}

void Exti::disable(TrigType type){
  switch (type)
  {
  case FALL:
    LL_EXTI_DisableFallingTrig_0_31(1<<_line);
    break;
  case RISE:
    LL_EXTI_DisableRisingTrig_0_31(1<<_line);
    break;
  case FALL_RISING:
    LL_EXTI_DisableRisingTrig_0_31(1<<_line);
    LL_EXTI_DisableFallingTrig_0_31(1<<_line);
    break;
  default:
    break;
  }
}

/**
 *@brief    EXTI 静态成员函数，在中断中调用，解析执行相关回调函数
 *@param    uint32_t id 对象地址，用来识别对象；IrqType irq_type 中断类型
 *@retval   NONE
*/
void Exti::_irq_handler(uint32_t pObj,uint8_t line)
{
  Exti *handler = (Exti*)pObj;  // 指向回调函数地址
  handler->_pirq[handler->_pin->read()].call();

}
/**
 * @brief   绑定中断的回调函数，产生中断后程序将执行callback_fun()
 * @param   void (*fptr)(void)类型函数的指针。
 *
 * @return  NONE
 */
void Exti::attach(void (*fptr)(void),TrigType type)
{
  if (type == FALL_RISING){
    _pirq[FALL].attach(fptr);
    _pirq[RISE].attach(fptr);
  }else{
    _pirq[type].attach(fptr);
  }
}


extern "C" {
  void EXTI4_15_IRQHandler(void)
  {
    for (uint8_t i = 4;i<=15;i++)
    {
      if (LL_EXTI_IsActiveFlag_0_31(1<<i) != RESET)
      {
        exti_handler(exti_irq_ids[i],i);
        LL_EXTI_ClearFlag_0_31(1<<i);
      }
    }
  }

  void EXTI0_1_IRQHandler(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
      exti_handler(exti_irq_ids[0],0);
    }else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
      exti_handler(exti_irq_ids[1],1);
    }
  }

  void EXTI2_3_IRQHandler(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
      exti_handler(exti_irq_ids[2],2);
    }else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
      exti_handler(exti_irq_ids[3],3);
    }
  }
}
