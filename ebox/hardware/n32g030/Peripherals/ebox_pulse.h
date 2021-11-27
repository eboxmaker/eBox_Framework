/**
  ******************************************************************************
  * @file    pwm.h
  * @author  shentq
  * @version V2.1
  * @date    2016/11/04
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

#ifndef __EBOX_PULSE_H
#define __EBOX_PULSE_H

#include "ebox_core.h"
#include "mcu.h"

/*
	1.֧��TIM1,2,3,4�µ�CH1-4ͨ��������16��ͨ��
	2.������ʱ����pwm�Ĳ����޸ĺ���������setfrq��������ı�ռ�ձ�,���ǻ�Ӱ��ͬ��ʱ��������ͨ���Ĺ�����
    3.pwm��ռ�õĶ�ʱ����ʱ��ԴƵ�ʿ���ͨ��get_timer_clock()��ã�
    4.PWM���Ƶ���������ͨ��get_max_frq()��ã�
    5.PWM��ǰ����ľ��ȿ���ͨ��get_accuracy()��ã�
	ע�⣺  1.��ʱû������֧��remap���š�
                ���ʹ��remap������Ҫ�Լ���ִ�����ʼ����������ִ����Ӧ��remap������Ȼ����Ҫremap�����ų�ʼ��ΪAF_PPģʽ
                ��ϸ������ο�pwm&remap.cpp
            2.ʹ������Ӧ��timxΪpwm����󣬸�tim�Ͳ����ٳ�ʼ��������Ĺ���
            3.��ʼ��pwm��ֻ�е���setDuty������Ż����pwm
            4.���duty���ڵ���1000����������ߵ�ƽ

    ��ʱ�����Ŷ�Ӧ�б�
    ͨ�� CH1  CH2  CH3  CH4
    TIM1 PA8  PA9  PA10 PA11
    TIM2 PA0  PA1  PA2  PA3
    TIM3 PA6  PA7  PA0  PA1
    TIM4 PB6  PB7  PB8  PB9
*/
/**
 * Modification History:
 * -shentq                  -version 1.2(2016/08/17)
 *      *�޸���set_dutyʱ���²����������
 *      *�޸���оƬ��Ƶ�䶯���¶�ʱ��PWMƵ�ʱ仯��bug��֧��������Ƶ����������
 *      *������get_timer_clock(),get_max_frq(),get_accuracy()���������ӿ�
 * -shentq                  -version 2.0(2016/11/4)
 *      *���ӶԶ�ʱ��1��֧��
 */

#define TIMxCH1 0x01
#define TIMxCH2 0x02
#define TIMxCH3 0x03
#define TIMxCH4 0x04

class Pulse
{
public:
    typedef struct {
        uint16_t    prescaler;//��Ƶ
        uint16_t    max_counter;//����ʱ��counter
        uint16_t    delay_counter;//����ʱ��counter
    }Config_t;
    
    Config_t cfg;
    Gpio        *pin;
public:
    Pulse(Gpio *pwm_pin);
    void begin(uint16_t percent);
    void end(void);

    void start();
    void stop();
    void set_oc_polarity(uint8_t flag);//�����������1���Ƚ�ƥ�������͵�ƽ��Ĭ�����ã���0���Ƚ�ƥ�������ߵ�ƽ

    void set_delay(uint16_t counter);
    void set_reload(uint16_t value);
    void set_phase_front(uint16_t percent);
    void set_phase_after(uint16_t percent);
    uint32_t get_timer_source_clock();

    float get_delay_us();
    void enable_pin(void);
    void disable_pin(void);
private:
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    duty;//����ռ�ձ�ֵ
    uint16_t    oc_polarity;

    void init_info(Gpio *pwm_pin);
    void base_init(uint16_t Period, uint16_t Prescaler);
    void oc_mode_init();
    //-------------------------------------------------------------
    uint16_t percent;
    uint16_t arr;


};
void analog_write(Gpio *pwm_pin, uint16_t duty);

#endif
