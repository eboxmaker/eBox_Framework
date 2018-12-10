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
��ʱ���жϼ�飺
    ���ļ��ṩ�˶�ʱ�����ж���ڡ����������ж�ʱ������thisָ�����tim_irq_ids[TIM_IRQ_ID_NUM]��
�û�����ͨ��Tim_It_Index����ѯÿһ����ʱ������thisָ����ڵ�indexֵ����ͨ��tim_irq_handler���͵�
������ڰ���ľ�̬������ڡ�ʵ����Ѱ�Ҷ��󲢶�λ�������������衣
*/


__IO uint16_t t1_overflow_times = 0;
__IO uint16_t t2_overflow_times = 0;
__IO uint16_t t3_overflow_times = 0;
__IO uint16_t t4_overflow_times = 0;


static tim_irq_handler irq_handler;
static uint32_t tim_irq_ids[TIM_IRQ_ID_NUM];//�������thisָ��

//index:��ʱ������thisָ�������б��е�����ֵ��ʵ�ֽ��ض��Ķ���󶨵��ض������
//handler����ľ�̬����
//id:�����thisָ��
int tim_irq_init(uint8_t index, tim_irq_handler handler, uint32_t id)
{
    tim_irq_ids[index] = id;//������thisָ�뱣�����б���
    irq_handler =  handler;//��ľ�̬����
    return 0;
}

extern "C" {

    void TIM1_BRK_UP_TRG_COM_IRQHandler(void)
    {
        if (LL_TIM_IsActiveFlag_UPDATE(TIM1))
        {
            t1_overflow_times++;
            irq_handler(tim_irq_ids[TIM1_IT_Update]);
            LL_TIM_ClearFlag_UPDATE(TIM1);
        }
    }

    void TIM1_CC_IRQHandler(void)
    {
        /* Check whether CC1 interrupt is pending */
        if (LL_TIM_IsActiveFlag_CC1(TIM1) == 1)
        {
            irq_handler(tim_irq_ids[TIM1_IT_CC1]);
            LL_TIM_ClearFlag_CC1(TIM1);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM1) == 1)
        {
            irq_handler(tim_irq_ids[TIM1_IT_CC2]);
            LL_TIM_ClearFlag_CC2(TIM1);
        }
        if (LL_TIM_IsActiveFlag_CC3(TIM1) == 1)
        {
            irq_handler(tim_irq_ids[TIM1_IT_CC3]);
            LL_TIM_ClearFlag_CC3(TIM1);
        }
    }

#ifdef TIM2
    void TIM2_IRQHandler(void)
    {
        /* Check whether update interrupt is pending */
        if (LL_TIM_IsActiveFlag_UPDATE(TIM2) == 1)
        {
            t2_overflow_times++;
            irq_handler(tim_irq_ids[TIM2_IT_Update]);
            LL_TIM_ClearFlag_UPDATE(TIM2);
        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM2) == 1)
        {
            irq_handler(tim_irq_ids[TIM2_IT_CC1]);
            LL_TIM_ClearFlag_CC1(TIM2);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM2) == 1)
        {
            irq_handler(tim_irq_ids[TIM2_IT_CC2]);
            LL_TIM_ClearFlag_CC2(TIM2);
        }
        if (LL_TIM_IsActiveFlag_CC3(TIM2) == 1)
        {
            irq_handler(tim_irq_ids[TIM2_IT_CC3]);
            LL_TIM_ClearFlag_CC3(TIM2);
        }
        if (LL_TIM_IsActiveFlag_CC4(TIM2) == 1)
        {
            irq_handler(tim_irq_ids[TIM2_IT_CC4]);
            LL_TIM_ClearFlag_CC3(TIM2);
        }
    }
#endif
#ifdef TIM3
    void TIM3_IRQHandler(void)
    {
        /* Check whether update interrupt is pending */
        if (LL_TIM_IsActiveFlag_UPDATE(TIM3) == 1)
        {
            t3_overflow_times++;
            irq_handler(tim_irq_ids[TIM3_IT_Update]);
            LL_TIM_ClearFlag_UPDATE(TIM3);
        }
        /* CC interrupt */
        if (LL_TIM_IsActiveFlag_CC1(TIM3) == 1)
        {
            irq_handler(tim_irq_ids[TIM3_IT_CC1]);
            LL_TIM_ClearFlag_CC1(TIM3);
        }
        if (LL_TIM_IsActiveFlag_CC2(TIM3) == 1)
        {
            irq_handler(tim_irq_ids[TIM3_IT_CC2]);
            LL_TIM_ClearFlag_CC2(TIM3);
        }
        if (LL_TIM_IsActiveFlag_CC3(TIM3) == 1)
        {
            irq_handler(tim_irq_ids[TIM3_IT_CC3]);
            LL_TIM_ClearFlag_CC3(TIM3);
        }
        if (LL_TIM_IsActiveFlag_CC4(TIM3) == 1)
        {
            irq_handler(tim_irq_ids[TIM3_IT_CC4]);
            LL_TIM_ClearFlag_CC3(TIM3);
        }
    }
#endif

    //    void TIM4_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM4 , TIM_IT_Update) == SET)
    //        {
    //            t4_overflow_times++;
    //            tim_irq_callback(TIM4_IT_Update);
    //            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_Update);
    //        }
    //        if(TIM_GetITStatus(TIM4 , TIM_IT_CC1) == SET)
    //        {
    //            tim_irq_callback(TIM4_IT_CC1);
    //            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC1);
    //        }
    //        if(TIM_GetITStatus(TIM4 , TIM_IT_CC2) == SET)
    //        {
    //            tim_irq_callback(TIM4_IT_CC2);
    //            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC2);
    //        }
    //        if(TIM_GetITStatus(TIM4 , TIM_IT_CC3) == SET)
    //        {
    //            tim_irq_callback(TIM4_IT_CC3);
    //            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC3);
    //        }
    //        if(TIM_GetITStatus(TIM4 , TIM_IT_CC4) == SET)
    //        {
    //            tim_irq_callback(TIM4_IT_CC4);
    //            TIM_ClearITPendingBit(TIM4 , TIM_FLAG_CC4);
    //        }
    //    }

    //    void TIM5_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_Update) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_Update);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_Update);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC1) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC1);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC1);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC2) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC2);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC2);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC3) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC3);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC3);
    //        }
    //        if(TIM_GetITStatus(TIM5 , TIM_IT_CC4) == SET)
    //        {
    //            tim_irq_callback(TIM5_IT_CC4);
    //            TIM_ClearITPendingBit(TIM5 , TIM_FLAG_CC4);
    //        }
    //    }
    //    void TIM6_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM6 , TIM_IT_Update) == SET)
    //        {
    //            tim_irq_callback(TIM6_IT_Update);
    //            TIM_ClearITPendingBit(TIM6 , TIM_FLAG_Update);
    //        }
    //    }
    //    void TIM7_IRQHandler(void)
    //    {
    //        if(TIM_GetITStatus(TIM7 , TIM_IT_Update) == SET)
    //        {
    //            tim_irq_callback(TIM7_IT_Update);
    //            TIM_ClearITPendingBit(TIM7 , TIM_FLAG_Update);
    //        }
    //    }

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
