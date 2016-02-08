/*
file   : in_capture.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#ifndef __IN_CAPTURE_H
#define __IN_CAPTURE_H
#include "common.h"

#define TIM_ICPOLARITY_FALLING TIM_ICPolarity_Falling
#define TIM_ICPOLARITY_RISING  TIM_ICPolarity_Rising
/*
1.֧��TIM2��3��4��ch1,2,3,4.����12��ͨ��
2.֧�ֲ������ڡ�Ƶ�ʡ��߼��÷�֧�ֲ���ռ�ձ�
3.��ʱ�����������ֵΪ0xffff,Ϊ��ͻ��������ƣ�
    �ڱ������У����ʹ����update����жϵ���overflow_event_process���Խ�������
    ��չ��2^32�������߲�����Χ������ʵ�����Ƶ�ʣ�1��Ƶ���������ڵ���60s���źţ�����źų��ȡ�
    ���ʹ��2��Ƶ���ɲ������ڵ���120s���źţ�����źų��ȡ��Դ����ơ�
4.get_captur()�ľ��Ȳ���ֵ
    //�˴�Ҫ��һ���򵥵�����ֵ,����Ϊ���Դ��²��Խ��
    //��Ƶϵ�� | ����ֵ | TIMʱ��Ƶ�� | ����Ƶ��
    //7200��Ƶ | 0      | 0.01M       |  1hz-100hz
    //720��Ƶ  | 1      | 0.10M       |  100-1K
    //72��Ƶ   | 2      | 1.00M       |  1K
    //64��Ƶ   | 3      | 1.125M      |  1K
    //36��Ƶ   | 5      | 2.00M       |  1K
    //32��Ƶ   | 6      | 2.25M       |  1K
    //18��Ƶ   | 11     | 4.00M       |  1K
    //16��Ƶ   | 11     | 4.50M       |  1K
    //9��Ƶ    | 18     | 8.00M       |  10K
    //8��Ƶ    | 21     | 9.00M       |  10K-50K
    //4��Ƶ    | 42     | 18.0M       |  10K-50K
    //2��Ƶ    | 84     | 36.0M       |  10K-50K
    //1��Ƶ    | 168    | 72.0M       |  10K-50K
5.���ڷ�Ƶϵ���������Ȳ����ļ����ϵ��Ҫ��ѭһ��ԭ���ڲ����������¾���ʹ�õͷ�Ƶϵ������TIMʱ�ӣ�ȥ������
�ص㣺
    �ڲ��õͷ�Ƶϵ����ʱ�򣬿��Ա�֤�������ȣ����ǻ�����ʱ�����Ƶ�ʣ���������cpu������
    �ڲ��ø߷�Ƶϵ����ʱ�򣬲������Ƚϵͣ����ǻή�Ͷ�ʱ�����Ƶ�ʣ���������cpu������
    stm32��72M��Ƶ�£���߿ɲ�160Khz���źš�����ٴ󣬽��޷�������
*/
typedef enum {
    IC_NONE = 0,
    IC_OVERFLOW = 1,
    IC_FAULT = 2
}IC_OVERFLOW_STATE_TYEP;

class IN_CAPTURE
{
    public:
        IN_CAPTURE(GPIO *capture_pin);
        void        begin(uint16_t prescaler);
        void        set_count(uint16_t count);
        void        set_polarity_falling();
        void        set_polarity_rising();
        void        overflow_event_process();
        uint32_t    get_capture();
        IC_OVERFLOW_STATE_TYEP        get_overflow_state();
        void        attch_ic_interrupt(void(*callback)(void));
        void        attch_update_interrupt(void(*callback)(void));

    private:
        GPIO        *capture_pin;
        TIM_TypeDef *TIMx;
        uint8_t     ch;
        uint16_t    period;//�������ֵ�����ڼ���ռ�ձ�
        uint16_t    prescaler;//�������ֵ�����ڼ���ռ�ձ�
        uint16_t    overflow_times;//�������
        IC_OVERFLOW_STATE_TYEP     overflow_state;//�����������������16λ�����IC_OVERFLOW���������32λ���:IC_FAULT
        uint8_t     polarity;
    
        void        init_info(GPIO *capture_pin);
        void        base_init(uint16_t Period,uint16_t Prescaler);

        uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
        void        (*_set_polarity)(TIM_TypeDef *TIMx,uint16_t TIM_OCPolarity);//����Ϊ�½��ز���
};
#endif
