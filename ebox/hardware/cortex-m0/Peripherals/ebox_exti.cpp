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

static exti_irq_handler irq_handler;
static uint32_t exti_irq_ids[16];

int exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
{
  exti_irq_ids[index] = id;
  irq_handler =  handler;
  return 0;
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
Exti::Exti(Gpio *pin, uint8_t trigger)
{
  this->pin = pin;
  this->trigger = trigger;
}

/**
 * @brief   初始化外部中断引脚的配置信息。
 * @param   NONE
 *
 * @return  NONE
 * @note    初始化会默认开启中断，如果用户想禁用中断，
 *          可以调用interrupt(DISABLE)关闭中断。
 */
void Exti::begin()
{
  uint8_t pinN = GETEXTILINE(pin->id);
  exti_line = 1 << pinN;
  exti_irq_init(pinN,(&Exti::_irq_handler),(uint32_t)this);

  pin->mode(INPUT);
  nvic(ENABLE,0,0);
}

void Exti::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
{
  nvic_dev_set_priority((uint32_t)exti_line,0,0,0);
  if (enable != DISABLE)
    nvic_dev_enable((uint32_t)exti_line,0);
  else
    nvic_dev_disable((uint32_t)exti_line,0);
}

/**
 * @brief   外部中断引脚的中断允许、禁止控制函数
 * @param   enable: 允许或者禁止中断
 *          - ENABLE: 允许该外部中断
 *          - DISABLE: 禁止该外部中断
 *
 * @return  NONE
 */
void Exti::interrupt(FunctionalState enable)
{
  LL_EXTI_InitTypeDef EXTI_initS;
  // connect exti line
  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_SYSCFG_SetEXTISource(GETEXTIPORT(pin->id), EXTISorce_MAP[GETEXTILINE(pin->id)]);

  switch (trigger)
  {
  case RISING:
    EXTI_initS.Trigger = LL_EXTI_TRIGGER_RISING;               //上升沿沿中断
    break;
  case FALLING:
    EXTI_initS.Trigger = LL_EXTI_TRIGGER_FALLING ;             //下降沿都中断
    break;
  case CHANGE:
    EXTI_initS.Trigger = LL_EXTI_TRIGGER_RISING_FALLING ;      //上升，下降沿都中断
    break;
  }
  EXTI_initS.Line_0_31 = exti_line;
  EXTI_initS.Mode = LL_EXTI_MODE_IT;
  EXTI_initS.LineCommand = enable;
  LL_EXTI_Init(&EXTI_initS);
}

void Exti::_irq_handler( uint32_t id)
{
  Exti *handler = (Exti*)id;
  handler->_irq.call();
}
/**
 * @brief   绑定中断的回调函数，产生中断后程序将执行callback_fun()
 * @param   void (*fptr)(void)类型函数的指针。
 *
 * @return  NONE
 */
void Exti::attach(void (*fptr)(void))
{
  if (fptr)
  {
    _irq.attach(fptr);
  }
}


extern "C" {
  void EXTI4_15_IRQHandler(void)
  {
    for (uint8_t i = 4;i<=15;i++)
    {
      if (LL_EXTI_IsActiveFlag_0_31(1<<i) != RESET)
      {
        irq_handler(exti_irq_ids[i]);
        LL_EXTI_ClearFlag_0_31(1<<i);
      }
    }
  }

  void EXTI0_1_IRQHandler(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
      irq_handler(exti_irq_ids[0]);
    }else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
      irq_handler(exti_irq_ids[1]);
    }
  }

  void EXTI2_3_IRQHandler(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
      irq_handler(exti_irq_ids[2]);
    }else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
      irq_handler(exti_irq_ids[3]);
    }
  }
}
