/**
  ******************************************************************************
  * @file    rtc.h
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

#ifndef __RTC_H
#define __RTC_H
#include "ebox_core.h"
#include "mcu.h"
/*
	1.�ṩһ��32λ��ѭ������,ÿ���1.
	2.һ���ж�Դ�������ж��¼�
	3.֧��һ������ʱ��
	4.֧��һ������
    
*/

class Rtc
{

public:
    int begin(uint8_t clock_source);//1:LSE;0:LSI���ʹ���ⲿ����
                                    //��������EOK
                                    //ʧ�ܺ�᷵��һ��ETIMEOUT����,���Զ�תΪ�ڲ�����

    void attach_overflow_interrupt(void (*cb_fun)(void));
    void attach_alarm_interrupt(void (*cb_fun)(void));
    void attach_sec_interrupt(void (*cb_fun)(void));

    void overflow_interrupt(FunctionalState state);
    void alarm_interrupt(FunctionalState state);
    void sec_interrupt(FunctionalState state);


    void set_counter(uint32_t count);
    void set_alarm(uint32_t count);
    uint32_t get_counter();


private:
    int    config(uint8_t flag);
    uint8_t is_config(uint16_t configFlag);
    void    set_config_flag(uint16_t configFlag);
    void    nvic(FunctionalState state);
};
class RtcClock
{
    public:
        uint8_t sec;
        uint8_t min;
        uint8_t hour;
    public:
        void begin();
        void sec_event()
        {
            count ++;
            count %=3600*24;
            sec  = (count % 60);
            min  = (count % 3600)/ 60;
            hour = (count / 3600);
        }
        void set_clock(uint8_t h,uint8_t m,uint8_t s)
        {
            count = h*3600 + m*60 + s;        
        }
    private:
       uint32_t count;
};
extern Rtc 	rtc;

#endif

