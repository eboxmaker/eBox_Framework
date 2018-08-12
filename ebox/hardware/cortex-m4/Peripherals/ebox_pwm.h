/**
  ******************************************************************************
  * @file    pwm.h
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

#ifndef __PWM_H
#define __PWM_H

#include "ebox_core.h"
#include "mcu.h"
/*
	1.֧��TIM2,3,4�µ�CH1-4ͨ����
	2.������ʱ����pwm�Ĳ����޸ĺ���������setfrq��������ı�ռ�ձ�,���ǻ�Ӱ��ͬ��ʱ��������ͨ���Ĺ�����
    3.pwm��ռ�õĶ�ʱ����ʱ��ԴƵ�ʿ���ͨ��get_timer_clock()��ã�
    4.PWM���Ƶ���������ͨ��get_max_frq()��ã�
    5.PWM��ǰ����ľ��ȿ���ͨ��get_accuracy()��ã�
	ע�⣺1.��ʱû������֧��remap���š�
					���ʹ��remap������Ҫ�Լ���ִ�����ʼ����������ִ����Ӧ��remap������
				2.ʹ������Ӧ��timxΪpwm����󣬸�tim�Ͳ����ٳ�ʼ��������Ĺ���
				3.��ʼ��pwm��ֻ�е���setDuty������Ż����pwm
				4.���duty���ڵ���1000����������ߵ�ƽ
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *�޸���set_dutyʱ���²����������
 *      *�޸���оƬ��Ƶ�䶯���¶�ʱ��PWMƵ�ʱ仯��bug��֧��������Ƶ����������
 *      *������get_timer_source_clock(),get_max_frq(),get_accuracy()���������ӿ�
 */
#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04
class Pwm
{
public:
    Pwm(Gpio *pwm_pin,TIM_TypeDef *timer,uint8_t ch);
    void begin(uint32_t frq, uint16_t duty);
    void set_frq(uint32_t frq);
    void set_duty(uint16_t duty);
    void set_oc_polarity(uint8_t flag);

    uint32_t get_timer_source_clock();
    uint32_t get_max_frq();
    void _set_duty(uint16_t duty);
    float   get_accuracy();

private:
    Gpio		*pwm_pin;
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    period;//�������ֵ�����ڼ���ռ�ձ�
    uint16_t    duty;//����ռ�ձ�ֵ
    uint16_t    oc_polarity;
    uint8_t     accuracy;

    void init_info(Gpio *pwm_pin);
    void base_init(uint32_t _period, uint32_t _prescaler);
//-------------------------------------------------------------

    uint8_t             af_timer_x;

};
void analog_write(Gpio *pwm_pin, uint16_t duty);

#endif
