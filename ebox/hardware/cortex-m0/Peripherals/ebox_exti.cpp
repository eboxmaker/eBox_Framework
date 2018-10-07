/**
  ******************************************************************************
  * @file    exti.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief
  *         2018/10/2         LQM
  *         ���������أ��½��ذ󶨲�ͬ���жϻص�����
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
#include "ebox_config.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  EXTI_DEBUG(...) DBG("[EXTI]  "),DBG(__VA_ARGS__)
#else
#define  EXTI_DEBUG(...)
#endif


// pin.id Ϊ8bit����4λ��ʶport 0-5,��4λΪio,0-15
#define GETEXTIPORT(a) ((uint32_t)a >>4 )
#define GETEXTILINE(a) ((uint32_t)1<<( a & 0x0f ))
#define GETPINNUMBER(a)(a & 0x0f)


// uint32_t pObj �����
typedef void (*exti_irq_handler)(uint32_t pObj);

static exti_irq_handler  exti_handler;   // ��������ָ�������ָ����ľ�̬��Ա
static uint32_t  exti_irq_ids[16];    	 // ��������ַ������̬��Աʶ����󣬲����ʶ������ͨ��Ա

void exti_irq_init(uint8_t index,exti_irq_handler handler,uint32_t id)
{
  exti_irq_ids[index] = id;       // ��������ַ
  exti_handler =  handler;        // ָ��ص�����
}

const uint32_t EXTISorce_MAP[]={
  LL_SYSCFG_EXTI_LINE0,LL_SYSCFG_EXTI_LINE1,LL_SYSCFG_EXTI_LINE2,LL_SYSCFG_EXTI_LINE3,
  LL_SYSCFG_EXTI_LINE4,LL_SYSCFG_EXTI_LINE5,LL_SYSCFG_EXTI_LINE6,LL_SYSCFG_EXTI_LINE7,
  LL_SYSCFG_EXTI_LINE8,LL_SYSCFG_EXTI_LINE9,LL_SYSCFG_EXTI_LINE10,LL_SYSCFG_EXTI_LINE11,
  LL_SYSCFG_EXTI_LINE12,LL_SYSCFG_EXTI_LINE13,LL_SYSCFG_EXTI_LINE14,LL_SYSCFG_EXTI_LINE15
};

/**
 * @brief    Exti���캯����ʵ����һ������
 * @param    *pin: ָ�����ⲿ�ж�����PA0~PG15
 * @return   NONE
 */
Exti::Exti(Gpio *pin)
{
  _pin = pin;
  _extiLine = GETEXTILINE(_pin->id);
  exti_irq_init(GETPINNUMBER(_pin->id),(&Exti::_irq_handler),(uint32_t)this);
}
/**
 * @brief    Exti���캯����ʵ����һ������
 * @param    mode: gpioģʽ��type �ж����ͣ�IT��EVENT,IT_EVENT��Ĭ��Ϊ�ж�
 * @return   NONE
 */
void Exti::begin(PIN_MODE mode,ExtiType type){
  _pin->mode(mode);

  LL_APB1_GRP2_EnableClock(LL_APB1_GRP2_PERIPH_SYSCFG);
  LL_SYSCFG_SetEXTISource(GETEXTIPORT(_pin->id), EXTISorce_MAP[GETPINNUMBER(_pin->id)]);

  EXTI_DEBUG("extiLine is %d , %d \r\n",_extiLine,GETEXTILINE(_pin->id));
  EXTI_DEBUG("pinNumber is %d \r\n",GETPINNUMBER(_pin->id));

  switch (type)
  {
  case IT:
    LL_EXTI_EnableIT_0_31(_extiLine);
    LL_EXTI_DisableEvent_0_31(_extiLine);
    break;
  case EVENT:
    LL_EXTI_EnableEvent_0_31(_extiLine);
    LL_EXTI_DisableIT_0_31(_extiLine);
    break;
  case IT_EVENT:
    LL_EXTI_EnableIT_0_31(_extiLine);
    LL_EXTI_EnableEvent_0_31(_extiLine);
    break;
  default:
    LL_EXTI_EnableIT_0_31(_extiLine);
    LL_EXTI_DisableEvent_0_31(_extiLine);
    break;
  }
}

//void Exti::nvic(FunctionalState enable, uint8_t preemption_priority, uint8_t sub_priority )
//{
//  nvic_dev_set_priority((uint32_t)exti_line,0,0,0);
//  if (enable != DISABLE)
//    nvic_dev_enable((uint32_t)exti_line,0);
//  else
//    nvic_dev_disable((uint32_t)exti_line,0);
//}

/**
  *@brief    ʹ���ж�
  *@param    trig ��������
  *@retval   NONE
  */
void Exti::enable(TrigType trig,uint32_t priority){
  switch (trig) // ʹ�ܴ�������
  {
  case FALL:
    LL_EXTI_EnableFallingTrig_0_31(_extiLine);
    LL_EXTI_DisableRisingTrig_0_31(_extiLine);
    break;
  case RISE:
    LL_EXTI_EnableRisingTrig_0_31(_extiLine);
    LL_EXTI_DisableFallingTrig_0_31(_extiLine);
    break;
  case FALL_RISING:
    LL_EXTI_EnableFallingTrig_0_31(_extiLine);
    LL_EXTI_EnableRisingTrig_0_31(_extiLine);
    break;
  default:
    break;
  }

  switch (GETPINNUMBER(_pin->id))   // ʹ�ܶ�Ӧ�ж�
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

/**
  *@brief    ��ֹ����
  *@param    trig ��������
  *@retval   NONE
  */
void Exti::disable(TrigType trig){
  switch (trig)
  {
  case FALL:
    LL_EXTI_DisableFallingTrig_0_31(_extiLine);
    break;
  case RISE:
    LL_EXTI_DisableRisingTrig_0_31(_extiLine);
    break;
  case FALL_RISING:
    LL_EXTI_DisableRisingTrig_0_31(_extiLine);
    LL_EXTI_DisableFallingTrig_0_31(_extiLine);
    break;
  default:
    break;
  }
}

/**
 *@brief    EXTI ��̬��Ա���������ж��е��ã�����ִ����ػص�����
 *@param    uint32_t id �����ַ������ʶ�����IrqType irq_type �ж�����
 *@retval   NONE
*/
void Exti::_irq_handler(uint32_t pObj)
{
  Exti *handler = (Exti*)pObj;  // ָ��ص�������ַ
  handler->_pirq[handler->_pin->read()].call();

}
/**
 * @brief   ���жϵĻص������������жϺ����ִ��callback_fun()
 * @param   void (*fptr)(void)���ͺ�����ָ�롣
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
        exti_handler(exti_irq_ids[i]);
        LL_EXTI_ClearFlag_0_31(1<<i);
      }
    }
  }

  void EXTI0_1_IRQHandler(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_0) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_0);
      exti_handler(exti_irq_ids[0]);
    }else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_1) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_1);
      exti_handler(exti_irq_ids[1]);
    }
  }

  void EXTI2_3_IRQHandler(void){
    if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_2) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_2);
      exti_handler(exti_irq_ids[2]);
    }else if (LL_EXTI_IsActiveFlag_0_31(LL_EXTI_LINE_3) != RESET)
    {
      LL_EXTI_ClearFlag_0_31(LL_EXTI_LINE_3);
      exti_handler(exti_irq_ids[3]);
    }
  }
}
