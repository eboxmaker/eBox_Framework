/**
  ******************************************************************************
  * @file    gtimer.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __GTIMER_H
#define __GTIMER_H

#include "common.h"

/*
	1.֧��tim1,2,3,4,5,6,7
    2.��ʱ����ʱ��ԴƵ�ʿ���ͨ��get_timer_clock()��ã�
    3.��ʱ������ж�Ƶ�ʿ���get_max_frq()��ã�
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *������TIM1��֧�֣�
 *      *������get_timer_clock(),get_max_frq()���������ӿ�
 *      *�޸���оƬ��Ƶ�䶯���¶�ʱ���ж�Ƶ�ʱ仯��bug��֧��������Ƶ����������
 */

class Timer
{
public:
    Timer(TIM_TypeDef *TIMx);
    void begin(uint32_t frq);
    void attach_interrupt(void(*callback)(void));
    void interrupt(FunctionalState enable);
    void start(void);
    void stop(void);
    void reset_frq(uint32_t frq);

    uint32_t get_timer_source_clock();
    uint32_t get_max_frq();

private:
    void base_init(uint16_t period, uint16_t prescaler);
    void set_reload(uint16_t auto_reload);
    void clear_count(void);
    TIM_TypeDef *_TIMx;

};
#endif
