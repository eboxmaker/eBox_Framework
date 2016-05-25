/*
file   : in_capture.cpp
author : shentq
version: V1.1
date   : 2016/4/6

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
3.Ĭ�ϲ����½��أ���ͨ���������ò�������ģʽ
4.��ʱ�����������ֵΪ0xffff,Ϊ��ͻ��������ƣ�
    �ڱ������У�ʹ����update����жϵ���tx_overflow_times���Խ�������
    ��չ��2^32�������߲�����Χ������ʵ�����Ƶ�ʣ�1��Ƶ���������ڵ���120s���źš�
    ���ʹ��2��Ƶ���ɲ������ڵ���240s���źš��Դ����ơ�
5.���ڷ�Ƶϵ���������Ȳ����ļ����ϵ��Ҫ��ѭһ��ԭ���ڲ����������¾���ʹ�õͷ�Ƶϵ������TIMʱ�ӣ�ȥ������
6.����get_capture()�Ͳ���ʱ����ת���Ĺ�ϵ��
    ʱ��(us)=get_capture()/(72/prescaler);
    ʱ��(ms)=get_capture()/(72000/prescaler);
    ʱ��(s)=get_capture()/(72000000/prescaler);
    ���ֱ��ʹ��get_zone_time_us()�������ֱ�ӵõ�һ������õ�ֵ������ʡȥ�ֹ�����Ĺ��̡�
    �˴��ṩ�����ֻ�ȡ���ؿ�ȵķ�����һ���ǰ���ʱ����������һ���ǰ�ʱ�䵥λע��������


7.���ʹ��ĳ����ʱ��ͨ���������벶����ö�ʱ������ͨ�������������벶��ģʽ������������Ϊ��������ģʽ
�ص㣺
    �ڲ��õͷ�Ƶϵ����ʱ�򣬿��Ա�֤�������ȣ����ǻ�����ʱ�����Ƶ�ʣ���������cpu������
    �ڲ��ø߷�Ƶϵ����ʱ�򣬲������Ƚϵͣ����ǻή�Ͷ�ʱ�����Ƶ�ʣ���������cpu������
    stm32��72M��Ƶ�£���߿ɲ�160Khz���źš�����ٴ󣬽��޷�������
*/

class IN_CAPTURE
{
public:
    IN_CAPTURE(GPIO *capture_pin);
    void        begin();//ʹ��Ĭ�ϲ�������Ƶϵ��Ϊ1���������Ϊ120s
    void        begin(uint16_t prescaler);
    void        set_count(uint16_t count);
    void        set_polarity_falling();
    void        set_polarity_rising();
    uint32_t    get_capture();//���������������ִ��Ч�ʸߣ�ȱ����Ҫ�û�����(us)=get_capture()/(72.0/prescaler);
    float       get_zone_time_us();//�����ڵ�Ƶ�ʲɼ����ŵ��������û�ȥ���㣬ȱ���Ƶ�ʵĵ��ø������㡣ռ��cpu����ʱ��
    void        attch_ic_interrupt(void(*callback)(void));
    void        attch_update_interrupt(void(*callback)(void));

    uint8_t     polarity;
private:
    GPIO        *capture_pin;
    TIM_TypeDef *TIMx;
    uint8_t     ch;
    uint16_t    period;//�������ֵ�����ڼ���ռ�ձ�
    uint16_t    prescaler;//�������ֵ�����ڼ���ռ�ձ�

    uint16_t    *overflow_times;
    uint32_t    last_value;

    void        init_info(GPIO *capture_pin);
    void        base_init(uint16_t Period, uint16_t Prescaler);

    uint16_t    (*_get_capture)(TIM_TypeDef *TIMx);
    void        (*_set_polarity)(TIM_TypeDef *TIMx, uint16_t TIM_OCPolarity); //����Ϊ�½��ػ��������ز���
};
#endif
