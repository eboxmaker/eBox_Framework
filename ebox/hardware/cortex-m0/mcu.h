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


// �պ����������κβ���������������ָ�븳ֵ����ֹ����ָ�� = nullʱ�����ã�����
  __STATIC_INLINE void nullFun(void){}
  typedef void (*fun_noPara_t)(void);           //�޲κ���ָ��
  typedef void (*fun_onePara_t)(unsigned int);  //����������ָ��


///////ȫ�ֱ���������///////////////////////////////////////////////
  extern __IO uint64_t millis_seconds;     //�ṩһ��mills()��Ч��ȫ�ֱ���������cpu���ÿ���
  // ��ʼ��
  extern void mcu_init(void);              //eboxϵͳ��ʼ��
  extern void SystemClock_Config(void);    //�ⲿʵ�ָú����������޸�ʱ��Դ�Լ�ʱ��Ƶ�ʡ�Ĭ�Ϲ�����48Mhz
  void mcu_reset(void);
//  uint32_t get_cpu_calculate_per_sec(void);//��ȡcpu�ڲ��ܲ���ϵͳ������µļ�������������ͳ��cpu����ʹ����
  // ʱ����ؽӿ�
  extern uint64_t mcu_micros(void);               //��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��us
  extern uint64_t mcu_millis(void);               //��ȡϵͳ��ǰ�����˶೤ʱ�䡣��λ��ms
  extern void mcu_delay_ms(uint32_t ms);//��ʱn���롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
  extern void mcu_delay_us(uint32_t us);//��ʱn΢�롣�������������ʹ�ã�������ֹ�����жϵ��������������1us��
  // systick�жϻص�����
  extern uint16_t attachSystickCallBack(fun_noPara_t fun,uint16_t multiple = 1); // ���жϻص������������������� = multiple * 1ms; ���65535

#define GetEndTime(timeOut)					(millis_seconds + timeOut)
// ��ʱ,����1 ���򷵻�0   �������millis()��ȡmillis_seconds,���Է�ֹ��Ϊ�ر��жϵ��³���������ʱ������
#define IsTimeOut(endTime,delay)		    ((uint32_t)(endTime - millis())>delay)      
      
#ifdef __cplusplus
}
#endif
#endif
