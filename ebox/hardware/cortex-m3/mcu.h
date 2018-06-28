/**
  ******************************************************************************
  * @file    core.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   ���ļ�Ϊebox�����ļ�,�ṩ�����ǳ���Ҫ�Ľӿ�,���Ա���׼��c�ļ����á�
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
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *�޸���оƬ��Ƶֻ������Ϊ72Mhz�����ƣ�֧��������Ƶ
 *      *������cpu����Ϣ��ȡ��������������ʱ�ӣ�оƬ������оƬ����Ӽӵ�����
 */


/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CORE_H
#define __CORE_H


#ifdef __cplusplus
extern "C" {
#endif
#include "ebox_type.h"
#include "mcu_config.h"
#include "rcc.h"
#include "nvic.h"


/*!< 2 bits for pre-emption priority
2 bits for subpriority */
#define NVIC_GROUP_CONFIG NVIC_PriorityGroup_2//�Ժ�NVIC_PriorityGroupConfig()��������Ҫ�ٱ����á����������Է�NVIC_GROUP_CONFIGֵ������


typedef void (*callback_fun_type)(void);

///////ȫ�ֱ���������///////////////////////////////////////////////
extern __IO uint64_t millis_seconds;//�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���

void mcu_init(void); //eboxϵͳ��ʼ��
void mcu_reset(void);
uint32_t get_cpu_calculate_per_sec(void);//��ȡcpu�ڲ��ܲ���ϵͳ������µļ�������������ͳ��cpu����ʹ����

uint64_t mcu_micros(void);//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��us
uint64_t mcu_millis(void);//��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��ms
void mcu_delay_ms(uint64_t ms);//��ʱn���롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
void mcu_delay_us(uint64_t us);//��ʱn΢�롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
void set_systick_user_event_per_sec(u16 frq);//�趨�û��ж��¼�����ӦƵ�ʡ�frq[1,1000],frq�����2�ı���
void attach_systick_user_event(void (*callback_fun)(void));//��systick�жϵ��ú���������Ƶ�����û�������Ƶ��Ϊ1hz-1000hz



#ifdef __cplusplus
}
#endif
#endif
