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

#ifndef __PWM_H
#define __PWM_H

#include "ebox_core.h"
#include "mcu.h"
#include "ebox_timer.h"
#include "ebox_gpio.h"

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



class Pwm:T_base{
public:
	Pwm(TIM_TypeDef *TIMx,mcuGpio *pin):T_base(TIMx){
		uint8_t _index;
		uint32_t t = (uint32_t)TIMx;
		//E_PinBase *_pin;
		//_pin = new E_PinBase(id);
		_index = getIndex(pin->id,t,TIM_MAP);
		//_pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
        pin->mode(TIM_MAP[_index]._pin_date,TIM_MAP[_index]._pin_af);
		_timx = TIMx;
		
		t = (TIM_MAP[_index]._periph_OR_ch) - (uint32_t)_timx;
		switch (t)
		{
		case TIMxCH1:
			_channel = LL_TIM_CHANNEL_CH1;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH1;
			break;
		case TIMxCH2:
			_channel = LL_TIM_CHANNEL_CH2;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH2;
			break;
		case TIMxCH3:
			_channel = LL_TIM_CHANNEL_CH3;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH3;
			break;
		case TIMxCH4:
			_channel = LL_TIM_CHANNEL_CH4;
			_OCsetCompare = &LL_TIM_OC_SetCompareCH4;
			break;
		}
	}
	void begin(uint32_t frq,uint16_t duty);

	void SetPorlicy(uint8_t porlicy);
	void SetFrequency(uint32_t frq);
	void SetDutyCycle(uint16_t duty);

	uint32_t GetMaxFrequency(void);
    
    // �˿�ƥ��
    void set_frq(uint32_t frq){SetFrequency(frq);};
    void set_duty(uint16_t duty){SetDutyCycle(duty);};//��֤�������ٶȷ������
    void set_oc_polarity(uint8_t flag){SetPorlicy(flag);};//�����������1���Ƚ�ƥ�������͵�ƽ��Ĭ�����ã���0���Ƚ�ƥ�������ߵ�ƽ

    uint32_t get_timer_source_clock(){return GetMaxFrequency();};
    uint32_t get_max_frq(){return GetMaxFrequency()/100;};
    float    get_accuracy();
private:
	uint32_t _channel;
	uint16_t _duty;		// ռ�ձ�
	uint8_t	 _accuracy; // ����

	pfun  _OCsetCompare;

	void _setMode(void);
};

//class Pwm
//{
//public:
//    Pwm(Gpio *pwm_pin);
//    void begin(uint32_t frq, uint16_t duty);
//    void set_frq(uint32_t frq);
//    void set_duty(uint16_t duty);//��֤�������ٶȷ������
//    void set_oc_polarity(uint8_t flag);//�����������1���Ƚ�ƥ�������͵�ƽ��Ĭ�����ã���0���Ƚ�ƥ�������ߵ�ƽ

//    uint32_t get_timer_source_clock();
//    uint32_t get_max_frq();
//    float    get_accuracy();

//private:
//    Gpio        *pwm_pin;
//    TIM_TypeDef *TIMx;
//    uint8_t     ch;
//    uint16_t    period;//�������ֵ�����ڼ���ռ�ձ�
//    uint16_t    duty;//����ռ�ձ�ֵ
//    uint16_t    oc_polarity;
//    uint8_t     accuracy;

//    void init_info(Gpio *pwm_pin);
//    void base_init(uint16_t Period, uint16_t Prescaler);
//    void _set_duty(uint16_t duty);
////-------------------------------------------------------------

//};
void analog_write(Gpio *pwm_pin, uint16_t duty);

#endif
