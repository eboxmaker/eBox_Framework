/**
  ******************************************************************************
  * @file    ebox_it.cpp
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

#include "ebox_core.h"
#include "mcu.h"

#include "ebox_timer_it.h"
/*
定时器中断简介：
    此文件提供了定时器的中断入口。定义了所有定时器对象this指针入口tim_irq_ids[TIM_IRQ_ID_NUM]。
用户可以通过Tim_It_Index来查询每一个定时器所有this指针入口的index值。并通过tim_irq_handler类型的
方法入口绑定类的静态方法入口。实现了寻找对象并定位方法的两个步骤。
*/


__IO uint16_t t1_overflow_times = 0;
__IO uint16_t t2_overflow_times = 0;
__IO uint16_t t3_overflow_times = 0;
__IO uint16_t t4_overflow_times = 0;


uint32_t master_count[4]  = {0};



static tim_irq_handler irq_handler;
static uint32_t tim_irq_ids[TIM_IRQ_ID_NUM];//保存对象this指针

//index:定时器保存this指针数组列表中的索引值，实现将特定的对象绑定到特定的入口
//handler：类的静态方法
//id:对象的this指针
int tim_irq_init(uint8_t index, tim_irq_handler handler, uint32_t id)
{
    tim_irq_ids[index] = id;//将对象this指针保存至列表中
    irq_handler =  handler;//类的静态方法
    return 0;
}

void tim_irq_callback(uint8_t index)
{
    irq_handler(tim_irq_ids[index]);//寻找到对象的回调函数入口
}


extern "C" {


    void TIM1_UP_IRQHandler(void)
    {

        if(TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
        {
            master_count[0] += 0x10000;
            t1_overflow_times++;
            tim_irq_callback(TIM1_IT_Update);
            TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM1, TIM_IT_CC1) == SET)
        {
            tim_irq_callback(TIM1_IT_CC1);
            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET)
        {
            tim_irq_callback(TIM1_IT_CC2);
            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM1, TIM_IT_CC3) == SET)
        {
            tim_irq_callback(TIM1_IT_CC3);
            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM1, TIM_IT_CC4) == SET)
        {
            tim_irq_callback(TIM1_IT_CC4);
            TIM_ClearITPendingBit(TIM1, TIM_FLAG_CC4);
        }
    }
    void TIM2_IRQHandler(void)
    {
        if((TIM2->SR & TIM_IT_Update) && (TIM2->DIER & TIM_IT_Update))
        {
            master_count[1] += 0x10000;
            t2_overflow_times++;
            tim_irq_callback(TIM2_IT_Update);
            TIM2->SR = (uint16_t)~TIM_IT_Update;
        }
        if((TIM2->SR & TIM_IT_CC1) && (TIM2->DIER & TIM_IT_CC1))
        {
            tim_irq_callback(TIM2_IT_CC1);
            TIM2->SR = (uint16_t)~TIM_IT_CC1;
        }
        if((TIM2->SR & TIM_IT_CC2) && (TIM2->DIER & TIM_IT_CC2))
        {
            tim_irq_callback(TIM2_IT_CC2);
            TIM2->SR = (uint16_t)~TIM_IT_CC2;
        }
        if((TIM2->SR & TIM_IT_CC3) && (TIM2->DIER & TIM_IT_CC3))
        {
            tim_irq_callback(TIM2_IT_CC3);
            TIM2->SR = (uint16_t)~TIM_IT_CC3;
        }
        if((TIM2->SR & TIM_IT_CC4) && (TIM2->DIER & TIM_IT_CC4))
        {
            tim_irq_callback(TIM2_IT_CC4);
            TIM2->SR = (uint16_t)~TIM_IT_CC4;
        }

    }
    void TIM3_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM3, TIM_IT_Update) == SET)
        {
            master_count[2] += 0x10000;
            t3_overflow_times++;
            tim_irq_callback(TIM3_IT_Update);
            TIM_ClearITPendingBit(TIM3, TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM3, TIM_IT_CC1) == SET)
        {
            tim_irq_callback(TIM3_IT_CC1);
            TIM_ClearITPendingBit(TIM2, TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM3, TIM_IT_CC2) == SET)
        {
            tim_irq_callback(TIM3_IT_CC2);
            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM3, TIM_IT_CC3) == SET)
        {
            tim_irq_callback(TIM3_IT_CC3);
            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM3, TIM_IT_CC4) == SET)
        {
            tim_irq_callback(TIM3_IT_CC4);
            TIM_ClearITPendingBit(TIM3, TIM_FLAG_CC4);
        }
    }
    void TIM4_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM4, TIM_IT_Update) == SET)
        {
            master_count[3] += 0x10000;
            t4_overflow_times++;
            tim_irq_callback(TIM4_IT_Update);
            TIM_ClearITPendingBit(TIM4, TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM4, TIM_IT_CC1) == SET)
        {
            tim_irq_callback(TIM4_IT_CC1);
            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM4, TIM_IT_CC2) == SET)
        {
            tim_irq_callback(TIM4_IT_CC2);
            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM4, TIM_IT_CC3) == SET)
        {
            tim_irq_callback(TIM4_IT_CC3);
            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM4, TIM_IT_CC4) == SET)
        {
            tim_irq_callback(TIM4_IT_CC4);
            TIM_ClearITPendingBit(TIM4, TIM_FLAG_CC4);
        }
    }

    void TIM5_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM5, TIM_IT_Update) == SET)
        {
            tim_irq_callback(TIM5_IT_Update);
            TIM_ClearITPendingBit(TIM5, TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM5, TIM_IT_CC1) == SET)
        {
            tim_irq_callback(TIM5_IT_CC1);
            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM5, TIM_IT_CC2) == SET)
        {
            tim_irq_callback(TIM5_IT_CC2);
            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM5, TIM_IT_CC3) == SET)
        {
            tim_irq_callback(TIM5_IT_CC3);
            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM5, TIM_IT_CC4) == SET)
        {
            tim_irq_callback(TIM5_IT_CC4);
            TIM_ClearITPendingBit(TIM5, TIM_FLAG_CC4);
        }
    }
    void TIM6_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM6, TIM_IT_Update) == SET)
        {
            tim_irq_callback(TIM6_IT_Update);
            TIM_ClearITPendingBit(TIM6, TIM_FLAG_Update);
        }
    }
    void TIM7_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM7, TIM_IT_Update) == SET)
        {
            tim_irq_callback(TIM7_IT_Update);
            TIM_ClearITPendingBit(TIM7, TIM_FLAG_Update);
        }
    }
































    void NMI_Handler(void)
    {
    }

    /**
      * @brief  This function handles Hard Fault exception.
      * @param  None
      * @retval None
      */
    void HardFault_Handler(void)
    {
        /* Go to infinite loop when Hard Fault exception occurs */
        //        hard_fault_isr();
        while (1)
        {
            // ebox_printf("123\r\n");

        }
    }

    /**
      * @brief  This function handles Memory Manage exception.
      * @param  None
      * @retval None
      */
    void MemManage_Handler(void)
    {
        /* Go to infinite loop when Memory Manage exception occurs */
        while (1)
        {
        }
    }

    /**
      * @brief  This function handles Bus Fault exception.
      * @param  None
      * @retval None
      */
    void BusFault_Handler(void)
    {
        /* Go to infinite loop when Bus Fault exception occurs */
        while (1)
        {
        }
    }

    /**
      * @brief  This function handles Usage Fault exception.
      * @param  None
      * @retval None
      */
    void UsageFault_Handler(void)
    {
        /* Go to infinite loop when Usage Fault exception occurs */
        while (1)
        {
        }
    }

    //void PendSV_Handler(void)
    //{
    //}
    /**
      * @brief  This function handles SVCall exception.
      * @param  None
      * @retval None
      */
    //void SVC_Handler(void)
    //{
    //}

    /**
      * @brief  This function handles Debug Monitor exception.
      * @param  None
      * @retval None
      */
    void DebugMon_Handler(void)
    {
    }

    /**
      * @brief  This function handles PendSVC exception.
      * @param  None
      * @retval None
      */
}

