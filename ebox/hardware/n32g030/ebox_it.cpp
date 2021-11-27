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


uint32_t master_count[4]  = {0};



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

void tim_irq_callback(uint8_t index)
{
    irq_handler(tim_irq_ids[index]);//Ѱ�ҵ�����Ļص��������
}


extern "C" {


    void TIM1_UP_IRQHandler(void)
    {

        if(TIM_GetIntStatus(TIM1, TIM_INT_UPDATE) == SET)
        {
            master_count[0] += 0x10000;
            t1_overflow_times++;
            tim_irq_callback(TIM1_IT_Update);
            TIM_ClrIntPendingBit(TIM1, TIM_FLAG_UPDATE);
            
        }
        if(TIM_GetIntStatus(TIM1, TIM_INT_CC1) == SET)
        {
            tim_irq_callback(TIM1_IT_CC1);
            TIM_ClrIntPendingBit(TIM1, TIM_FLAG_CC1);
        }
        if(TIM_GetIntStatus(TIM1, TIM_INT_CC2) == SET)
        {
            tim_irq_callback(TIM1_IT_CC2);
            TIM_ClrIntPendingBit(TIM1, TIM_FLAG_CC2);
        }
        if(TIM_GetIntStatus(TIM1, TIM_INT_CC3) == SET)
        {
            tim_irq_callback(TIM1_IT_CC3);
            TIM_ClrIntPendingBit(TIM1, TIM_FLAG_CC3);
        }
        if(TIM_GetIntStatus(TIM1, TIM_INT_CC4) == SET)
        {
            tim_irq_callback(TIM1_IT_CC4);
            TIM_ClrIntPendingBit(TIM1, TIM_FLAG_CC4);
        }
    }
    void TIM2_IRQHandler(void)
    {
//        if((TIM2->SR & TIM_INT_UPDATE) && (TIM2->DIER & TIM_INT_UPDATE))
//        {
//            master_count[1] += 0x10000;
//            t2_overflow_times++;
//            tim_irq_callback(TIM2_IT_Update);
//            TIM2->SR = (uint16_t)~TIM_INT_UPDATE;
//        }
//        if((TIM2->SR & TIM_IT_CC1) && (TIM2->DIER & TIM_IT_CC1))
//        {
//            tim_irq_callback(TIM2_IT_CC1);
//            TIM2->SR = (uint16_t)~TIM_IT_CC1;
//        }
//        if((TIM2->SR & TIM_IT_CC2) && (TIM2->DIER & TIM_IT_CC2))
//        {
//            tim_irq_callback(TIM2_IT_CC2);
//            TIM2->SR = (uint16_t)~TIM_IT_CC2;
//        }
//        if((TIM2->SR & TIM_IT_CC3) && (TIM2->DIER & TIM_IT_CC3))
//        {
//            tim_irq_callback(TIM2_IT_CC3);
//            TIM2->SR = (uint16_t)~TIM_IT_CC3;
//        }
//        if((TIM2->SR & TIM_IT_CC4) && (TIM2->DIER & TIM_IT_CC4))
//        {
//            tim_irq_callback(TIM2_IT_CC4);
//            TIM2->SR = (uint16_t)~TIM_IT_CC4;
//        }

    }
    void TIM3_IRQHandler(void)
    {
        if(TIM_GetIntStatus(TIM3, TIM_INT_UPDATE) == SET)
        {
            master_count[2] += 0x10000;
            t3_overflow_times++;
            tim_irq_callback(TIM3_IT_Update);
            TIM_ClrIntPendingBit(TIM3, TIM_FLAG_UPDATE);
        }
        if(TIM_GetIntStatus(TIM3, TIM_INT_CC1) == SET)
        {
            tim_irq_callback(TIM3_IT_CC1);
            TIM_ClrIntPendingBit(TIM3, TIM_FLAG_CC1);
        }
        if(TIM_GetIntStatus(TIM3, TIM_INT_CC2) == SET)
        {
            tim_irq_callback(TIM3_IT_CC2);
            TIM_ClrIntPendingBit(TIM3, TIM_FLAG_CC2);
        }
        if(TIM_GetIntStatus(TIM3, TIM_INT_CC3) == SET)
        {
            tim_irq_callback(TIM3_IT_CC3);
            TIM_ClrIntPendingBit(TIM3, TIM_FLAG_CC3);
        }
        if(TIM_GetIntStatus(TIM3, TIM_INT_CC4) == SET)
        {
            tim_irq_callback(TIM3_IT_CC4);
            TIM_ClrIntPendingBit(TIM3, TIM_FLAG_CC4);
        }
    }
    void TIM4_IRQHandler(void)
    {
//        if(TIM_GetIntStatus(TIM4, TIM_INT_UPDATE) == SET)
//        {
//            master_count[3] += 0x10000;
//            t4_overflow_times++;
//            tim_irq_callback(TIM4_IT_Update);
//            TIM_ClrIntPendingBit(TIM4, TIM_FLAG_UPDATE);
//        }
//        if(TIM_GetIntStatus(TIM4, TIM_INT_CC1) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC1);
//            TIM_ClrIntPendingBit(TIM4, TIM_FLAG_CC1);
//        }
//        if(TIM_GetIntStatus(TIM4, TIM_INT_CC2) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC2);
//            TIM_ClrIntPendingBit(TIM4, TIM_FLAG_CC2);
//        }
//        if(TIM_GetIntStatus(TIM4, TIM_INT_CC3) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC3);
//            TIM_ClrIntPendingBit(TIM4, TIM_FLAG_CC3);
//        }
//        if(TIM_GetIntStatus(TIM4, TIM_INT_CC4) == SET)
//        {
//            tim_irq_callback(TIM4_IT_CC4);
//            TIM_ClrIntPendingBit(TIM4, TIM_FLAG_CC4);
//        }
    }

    void TIM5_IRQHandler(void)
    {
//        if(TIM_GetIntStatus(TIM5, TIM_INT_UPDATE) == SET)
//        {
//            tim_irq_callback(TIM5_IT_Update);
//            TIM_ClrIntPendingBit(TIM5, TIM_FLAG_UPDATE);
//        }
//        if(TIM_GetIntStatus(TIM5, TIM_INT_CC1) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC1);
//            TIM_ClrIntPendingBit(TIM5, TIM_FLAG_CC1);
//        }
//        if(TIM_GetIntStatus(TIM5, TIM_INT_CC2) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC2);
//            TIM_ClrIntPendingBit(TIM5, TIM_FLAG_CC2);
//        }
//        if(TIM_GetIntStatus(TIM5, TIM_INT_CC3) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC3);
//            TIM_ClrIntPendingBit(TIM5, TIM_FLAG_CC3);
//        }
//        if(TIM_GetIntStatus(TIM5, TIM_INT_CC4) == SET)
//        {
//            tim_irq_callback(TIM5_IT_CC4);
//            TIM_ClrIntPendingBit(TIM5, TIM_FLAG_CC4);
//        }
    }
    void TIM6_IRQHandler(void)
    {
        if(TIM_GetIntStatus(TIM6, TIM_INT_UPDATE) == SET)
        {
            tim_irq_callback(TIM6_IT_Update);
            TIM_ClrIntPendingBit(TIM6, TIM_FLAG_UPDATE);
        }
    }
    void TIM7_IRQHandler(void)
    {
//        if(TIM_GetIntStatus(TIM7, TIM_INT_UPDATE) == SET)
//        {
//            tim_irq_callback(TIM7_IT_Update);
//            TIM_ClrIntPendingBit(TIM7, TIM_FLAG_UPDATE);
//        }
    }
    void TIM8_UP_IRQHandler(void)
    {

        if(TIM_GetIntStatus(TIM8, TIM_INT_UPDATE) == SET)
        {
            master_count[0] += 0x10000;
            t1_overflow_times++;
            tim_irq_callback(TIM1_IT_Update);
            TIM_ClrIntPendingBit(TIM8, TIM_FLAG_UPDATE);
            
        }
        if(TIM_GetIntStatus(TIM8, TIM_INT_CC1) == SET)
        {
            tim_irq_callback(TIM1_IT_CC1);
            TIM_ClrIntPendingBit(TIM8, TIM_FLAG_CC1);
        }
        if(TIM_GetIntStatus(TIM8, TIM_INT_CC2) == SET)
        {
            tim_irq_callback(TIM1_IT_CC2);
            TIM_ClrIntPendingBit(TIM8, TIM_FLAG_CC2);
        }
        if(TIM_GetIntStatus(TIM8, TIM_INT_CC3) == SET)
        {
            tim_irq_callback(TIM1_IT_CC3);
            TIM_ClrIntPendingBit(TIM8, TIM_FLAG_CC3);
        }
        if(TIM_GetIntStatus(TIM8, TIM_INT_CC4) == SET)
        {
            tim_irq_callback(TIM1_IT_CC4);
            TIM_ClrIntPendingBit(TIM8, TIM_FLAG_CC4);
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
            // ebox_printf("123\n");

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

