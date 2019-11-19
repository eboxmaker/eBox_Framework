/**
  ******************************************************************************
  * @file    calendar.h
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
#ifndef __CLOCK_H
#define __CLOCK_H
#include "ebox_core.h"
#include "basicRtc.h"
/******************************************************
��������GPS,BDģ������ʱ��
Ҳ��������RTC���ж�����ʱ��
---------------------------------
1.��׼��ʱ�ӷ���
2.ʱ������
3.����ʱ����
4.����ʱ�ַ�����ʱ���ַ����������ַ���
5.���ڵļ���
---------------------------------
�����е��������λ�����档ֻ�ܴ�2000�����
*******************************************************/


#define TIMEZONE 8
class Calendar
{
public:
    DateTime_t dt;

public:
    Calendar(BasicRtc &_rtc);
    void begin();
    void loop();

public:
    BasicRtc *rtc;
    uint32_t last_mark_time;
};


#endif

