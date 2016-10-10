/**
  ******************************************************************************
  * @file    rtc.cpp
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
#include "ebox_rtc.h"
#define RTC_CFG_FLAG 0XA5A5
#include "ebox.h"
Rtc rtc;
//callback_fun_type rtc_callback_table[3];//
static uint32_t rtc_irq_id;
static rtc_irq_handler irq_handler;

int Rtc::begin(uint8_t clock_source)
{

    int ret = EOK;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);
    /* Reset Backup Domain */
//    BKP_DeInit();


    if(is_config(RTC_CFG_FLAG) == 0)
    {
        if(config(clock_source) != EOK)
        {
            config(0);
            ret = EPARA;
        }
    
        set_config_flag(RTC_CFG_FLAG);
    }
	else
	{
		/* Check if the Power On Reset flag is set */
		if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
		{
			uart1.printf("\r\n\n Power On Reset occurred....");
		}
		/* Check if the Pin Reset flag is set */
		else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
		{
			uart1.printf("\r\n\n External Reset occurred....");
		}
		
		uart1.printf("\r\n No need to configure RTC....");
		uart1.printf("\r\n step 0....");
		/* Wait for RTC registers synchronization */
		RTC_WaitForSynchro();
		uart1.printf("\r\n step 1....");
		
		/* Enable the RTC Second */
		RTC_ITConfig(RTC_IT_SEC, ENABLE);
		uart1.printf("\r\n step 2....");
		
		/* Wait until last write operation on RTC registers has finished */
		RTC_WaitForLastTask();
        
		uart1.printf("\r\n step 3....");
	}
	
		rtc_irq_init(Rtc::_irq_handler, (uint32_t)this);
    return ret;
}
int Rtc::config(uint8_t flag)
{

    int ret;
    uint32_t i=0;
    /* Enable PWR and BKP clocks */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);

    /* Allow access to BKP Domain */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset Backup Domain */
    BKP_DeInit();
    
    if(flag == 1)
    {
        /* Enable LSE */
        RCC_LSEConfig(RCC_LSE_ON);
        /* Wait till LSE is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
        {
            i++;
            if(i >= 0x1fffff)
            {
                ret = ETIMEOUT;
                return ret;
            }
        }            
        /* Select LSE as RTC Clock Source */
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
    }
    else
    {
        RCC_LSICmd(ENABLE);
        /* Wait till LSI is ready */
        while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);
        RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

        /* Enable RTC Clock */
        RCC_RTCCLKCmd(ENABLE);

    }

    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();

    /* Enable the RTC Second */
    RTC_ITConfig(RTC_IT_SEC, ENABLE);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(flag == 1)
    {
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(32767); /* RTC period = RTCCLK/RTC_PR = (32.768 KHz)/(32767+1) */
    }
    else
    {
        /* Set RTC prescaler: set RTC period to 1sec */
        RTC_SetPrescaler(39999); /* RTC period = RTCCLK/RTC_PR = (40 KHz)/(39999+1) */

    }

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
       
    
    nvic(ENABLE);

    return EOK;
}
uint8_t Rtc::is_config(uint16_t configFlag)
{
    return (BKP_ReadBackupRegister(BKP_DR1) == configFlag);
}
void Rtc::set_config_flag(uint16_t configFlag)
{
    BKP_WriteBackupRegister(BKP_DR1, configFlag);
}
void Rtc::set_counter(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetCounter(count);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}
void Rtc::set_alarm(uint32_t count)
{
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    /* Change the current time */
    RTC_SetAlarm(count);

    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
}


void Rtc::nvic(FunctionalState state)
{

    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure one bit for preemption priority */
    //	NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

    /* Enable the RTC Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = state;
    NVIC_Init(&NVIC_InitStructure);
}

void Rtc::attach_sec_interrupt(void (*cb_fun)(void))
{
   // rtc_callback_table[0] = cb_fun;
}
void Rtc::attach_alarm_interrupt(void (*cb_fun)(void))
{
    //rtc_callback_table[1] = cb_fun;
}
void Rtc::attach_overflow_interrupt(void (*cb_fun)(void))
{
   // rtc_callback_table[2] = cb_fun;
}

void Rtc::sec_interrupt(FunctionalState state)
{
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<0);
    else
         RTC->CRH &= ~(1<<0);
}

void Rtc::alarm_interrupt(FunctionalState state)
{
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<1);
    else
         RTC->CRH &= ~(1<<1);
}

void Rtc::overflow_interrupt(FunctionalState state)
{
     
    /* Wait for RTC registers synchronization */
    RTC_WaitForSynchro();
    /* Wait until last write operation on RTC registers has finished */
    RTC_WaitForLastTask();
    if(state == ENABLE)
        RTC->CRH |= (1<<2);
    else
         RTC->CRH &= ~(1<<2);
}


void Rtc::attach(void (*fptr)(void), Rtc_IrqType type) {
    if (fptr) {
        _irq[type].attach(fptr);
    }
}


void Rtc::_irq_handler(uint32_t id, Rtc_IrqType irq_type) {
    Rtc *handler = (Rtc*)id;
    handler->_irq[irq_type].call();
}

uint32_t Rtc::get_counter()
{
    return RTC_GetCounter();
};
extern "C" {
    void RTC_IRQHandler(void)
    {
        if (RTC_GetITStatus(RTC_IT_SEC) != RESET)
        {
						irq_handler(rtc_irq_id,Sec_Irq);
            RTC_ClearITPendingBit(RTC_IT_SEC);
        }
        if (RTC_GetITStatus(RTC_IT_ALR) != RESET)
        {
						irq_handler(rtc_irq_id,Alr_Irq);
            RTC_ClearITPendingBit(RTC_IT_ALR);
        }
        if (RTC_GetITStatus(RTC_IT_OW) != RESET)
        {
						irq_handler(rtc_irq_id,Ow_Irq);
            RTC_ClearITPendingBit(RTC_IT_OW);
        }
    }
		
		void rtc_irq_init(rtc_irq_handler handler, uint32_t id)
		{
				irq_handler = handler;
				rtc_irq_id = id;
		}
}
