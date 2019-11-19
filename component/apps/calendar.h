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
可以用于GPS,BD模块驱动时间
也可以用于RTC秒中断驱动时间
---------------------------------
1.标准的时钟服务
2.时区调整
3.倒计时闹钟
4.倒计时字符串、时间字符串、日期字符串
5.星期的计算
---------------------------------
日历中的年份以两位数代替。只能从2000年记起
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

