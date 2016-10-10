/**
  ******************************************************************************
  * @file    ebox_it.cpp
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


/* Includes ------------------------------------------------------------------*/

#include "ebox_common.h"
#include "ebox_timer_it.h"
__IO uint16_t t1_overflow_times = 0;
__IO uint16_t t2_overflow_times = 0;
__IO uint16_t t3_overflow_times = 0;
__IO uint16_t t4_overflow_times = 0;

extern "C" {


    void TIM1_UP_IRQHandler(void)
    {

        if(TIM_GetITStatus(TIM1 , TIM_IT_Update) == SET)
        {
            t1_overflow_times++;
						tim_irq_callback(TIM1_IT_Update);
            TIM_ClearITPendingBit(TIM1 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM1 , TIM_IT_CC1) == SET)
        {
						tim_irq_callback(TIM1_IT_CC1);
            TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM1 , TIM_IT_CC2) == SET)
        {
						tim_irq_callback(TIM1_IT_CC2);
            TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM1 , TIM_IT_CC3) == SET)
        {
						tim_irq_callback(TIM1_IT_CC3);
            TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM1 , TIM_IT_CC4) == SET)
        {
						tim_irq_callback(TIM1_IT_CC4);
            TIM_ClearITPendingBit(TIM1 , TIM_FLAG_CC4);
        }
    }
    void TIM2_IRQHandler(void)
    {

        if(TIM_GetITStatus(TIM2 , TIM_IT_Update) == SET)
        {
            t2_overflow_times++;
						tim_irq_callback(TIM2_IT_Update);
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC1) == SET)
        {
						tim_irq_callback(TIM2_IT_CC1);
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC2) == SET)
        {
						tim_irq_callback(TIM2_IT_CC2);
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC3) == SET)
        {
						tim_irq_callback(TIM2_IT_CC3);
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM2 , TIM_IT_CC4) == SET)
        {
						tim_irq_callback(TIM2_IT_CC4);
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC4);
        }


    }
    void TIM3_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM3 , TIM_IT_Update) == SET)
        {
            t3_overflow_times++;
						tim_irq_callback(TIM3_IT_Update);
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC1) == SET)
        {
						tim_irq_callback(TIM3_IT_CC1);
            TIM_ClearITPendingBit(TIM2 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC2) == SET)
        {
						tim_irq_callback(TIM3_IT_CC2);
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC3) == SET)
        {
						tim_irq_callback(TIM3_IT_CC3);
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM3 , TIM_IT_CC4) == SET)
        {
						tim_irq_callback(TIM3_IT_CC4);
            TIM_ClearITPendingBit(TIM3 , TIM_FLAG_CC4);
        }
    }
    void TIM4_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
        {
            t4_overflow_times++;
						tim_irq_callback(TIM4_IT_Update);
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC1) == SET)
        {
						tim_irq_callback(TIM4_IT_CC1);
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC2) == SET)
        {
						tim_irq_callback(TIM4_IT_CC2);
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC3) == SET)
        {
						tim_irq_callback(TIM4_IT_CC3);
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM4 , TIM_IT_CC4) == SET)
        {
						tim_irq_callback(TIM4_IT_CC4);
            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC4);
        }
    }

    void TIM5_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM5 , TIM_IT_Update) == SET)
        {
						tim_irq_callback(TIM5_IT_Update);
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC1) == SET)
        {
						tim_irq_callback(TIM5_IT_CC1);
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC1);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC2) == SET)
        {
						tim_irq_callback(TIM5_IT_CC2);
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC2);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC3) == SET)
        {
						tim_irq_callback(TIM5_IT_CC3);
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC3);
        }
        if(TIM_GetITStatus(TIM5 , TIM_IT_CC4) == SET)
        {
						tim_irq_callback(TIM5_IT_CC4);
            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC4);
        }
    }
    void TIM6_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM6 , TIM_IT_Update) == SET)
        {
						tim_irq_callback(TIM6_IT_Update);
            TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
        }
    }
    void TIM7_IRQHandler(void)
    {
        if(TIM_GetITStatus(TIM7 , TIM_IT_Update) == SET)
        {
						tim_irq_callback(TIM7_IT_Update);
            TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);
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
        while (1)
        {
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

