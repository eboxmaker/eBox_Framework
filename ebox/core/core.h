/*
file   : core.h
author : shentq
version: V1.1
date   : 2016/4/6
brief  : analog read function

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
/*
���ļ�Ϊebox�����ļ���
�ṩ�����ǳ���Ҫ�Ľӿڡ�
���Ա���׼��c�ļ����á�
*/

#ifndef __CORE_H
#define __CORE_H
#ifdef __cplusplus
extern "C" {
#endif
#include "stm32f10x.h"

#define interrupts() 		__enable_irq()//���������ж�
#define no_interrupts() 	__disable_irq()//��ֹ�����ж�

/*!< 2 bits for pre-emption priority
2 bits for subpriority */
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2//�Ժ�NVIC_PriorityGroupConfig()��������Ҫ�ٱ����á����������Է�NVIC_GROUP_CONFIGֵ������


typedef void (*callback_fun_type)(void);

///////ȫ�ֱ���������///////////////////////////////////////////////
extern __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���

void ebox_init(void); //eboxϵͳ��ʼ��
uint32_t get_cpu_calculate_per_sec();//��ȡcpu�ڲ��ܲ���ϵͳ������µļ�������������ͳ��cpu����ʹ����

uint64_t micros(void);//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��us
uint64_t millis(void);//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��ms
void delay_ms(uint64_t ms);//��ʱn���롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
void delay_us(uint64_t us);//��ʱn΢�롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
void set_systick_user_event_per_sec(u16 frq);//�趨�û��ж��¼�����ӦƵ�ʡ�frq[1,1000],frq�����2�ı���
void attach_systick_user_event(void (*callback_fun)(void));//��systick�жϵ��ú���������Ƶ�����û�������Ƶ��Ϊ1hz-1000hz

#ifdef __cplusplus
}
#endif
#endif
