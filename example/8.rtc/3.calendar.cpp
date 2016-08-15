/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */


#include "ebox.h"
#include "calendar.h"

Calendar calendar;

void rtc_sec_event()
{
    calendar.sec_process();
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    rtc.begin(1);
    rtc.attach_sec_interrupt(rtc_sec_event);
    rtc.sec_interrupt(ENABLE);
    calendar.begin();
    calendar.set(16,12,31,23,00,00);
    calendar.alarm_set(17,1,2,0,0,0);
}
int main(void)
{
    setup();
    while(1)
    {       
        uart1.printf("date:%04d-%02d-%02d\r\nweek:%02d\r\n",calendar.dt.year+2000,calendar.dt.month,calendar.dt.date,calendar.dt.week);
        uart1.printf("time:%02d:%02d:%02d\r\n",calendar.dt.hour,calendar.dt.min,calendar.dt.sec);
        uart1.printf("date string:%s\r\n",calendar.date_string);
        uart1.printf("time string:%s\r\n",calendar.time_string);
        uart1.printf("cutdown dhms string:%s\r\n",calendar.cutdown_dddhhmmss_string);
        uart1.printf("cutdown  hms string:%s\r\n\r\n",calendar.cutdown_hhmmss_string);
		delay_ms(1000);
    }

}


