/*
file   : core.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/


#include "ebox.h"
#include "common.h"


 __IO uint64_t millis_seconds;
 __IO uint32_t cpu_calculate_per_sec;


#define systick_over_flow_value 9000//��ֵȡֵ��Χ��100-9000������ƵΪ72Mhz�������

void ebox_init(void)
{
    
    SysTick_Config(systick_over_flow_value);//  ÿ�� (nhz/9,000,000)s����һ���ж�
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//9Mhz��systemticks clock��
    //ͳ��cpu��������//////////////////
    cpu_calculate_per_sec = 0;
    millis_seconds = 0;
    do{
        cpu_calculate_per_sec++;//ͳ��cpu��������
    }while(millis_seconds < 100);
    cpu_calculate_per_sec = cpu_calculate_per_sec * 10;
    ////////////////////////////////
    ADC1_init();
    
    NVIC_PriorityGroupConfig(NVIC_GROUP_CONFIG);

    //��pb4Ĭ������ΪIO�ڣ�����jtag
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
     GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);
    sys.get_chip_info();

}

uint64_t millis( void )
{
  return millis_seconds;
}
uint64_t micros(void)
{
    uint64_t ret;
    no_interrupts();
    if((SysTick->CTRL & 0x8000))//�����ʱ������systick�������Ҫ��millis_secend���в���
    {
        millis_seconds++;
    }
    ret = millis_seconds * 1000 + (9000 - SysTick->VAL / 9);
    interrupts();
    return  ret;
}

//void delay_ms(uint64_t ms)
//{	 	

//    if(ms <= 0) return;
//    uint64_t end = millis() + ms*(9000/systick_over_flow_value);
//    uint64_t systick = SysTick->VAL;

//    while (millis() < end) {
//        ;
//        }
//    while(SysTick->VAL > systick)
//        {
//            ;
//        }
//}   

//void delay_us(uint16_t us)
//{		 
//	uint32_t systick = SysTick->VAL;
//	uint16_t count;
//	if(count == 0)return;
//	
//	count = us * 9;
//	if(count>systick_over_flow_value - 1)count = systick_over_flow_value-1;
//	no_interrupts();
//	if(systick < count)
//	{
//		count  = ((systick_over_flow_value-5)  + systick - count);///
//		while(SysTick->VAL <= count)
//		{
//			;
//		}
//		if(count == 0)
//			count = 1;

//		while(SysTick->VAL > count)
//		{
//			;
//		}
//        
////		millis_seconds++;//�����������
//	}
//	else
//	{
//		count  =  systick - count;
//		if(count == 0)
//			count = 1; 
//		while(SysTick->VAL > count)
//		{
//			;
//		}
//	}
//	interrupts();
//}

    								   

void delay_ms(uint64_t ms)
{	 	
    uint64_t end ;
    no_interrupts();
    end = micros() + ms*1000 - 3;
    while(micros() < end);
    interrupts();
}   
void delay_us(uint64_t us)
{	 	
    no_interrupts();
    uint64_t end = micros() + us - 3;
    while(micros() < end);
    interrupts();
} 
void delayms(uint64_t ms)
{	 	
    uint64_t end ;
    end = micros() + ms*1000 - 3;
    while(micros() < end);
}   
void delayus(uint64_t us)
{	 	
    uint64_t end = micros() + us - 3;
    while(micros() < end);
} 

extern "C"{
    
void ebox_printf(const char *fmt,...)
{
    DBG(fmt);
}

callback_fun_type sys_ticks_cb_table[1] = {0};
	
	
void attch_sys_ticks_interrupt(void (*callback_fun)(void))
{
    sys_ticks_cb_table[0] = callback_fun;

}
	
void SysTick_Handler(void)//systick�ж�
{

    no_interrupts();
    millis_seconds++;
    if(sys_ticks_cb_table[0] != 0)
        sys_ticks_cb_table[0]();
    interrupts();
}

//void PendSV_Handler(void)
//{
//}

//STM32�����ж�////////////////////////////
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
