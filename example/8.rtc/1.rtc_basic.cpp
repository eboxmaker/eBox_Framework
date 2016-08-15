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

void rtcsecit()
{
   uart1.printf("count = %x\r\n", rtc.get_counter());
}
void alarm_event()
{
    uart1.printf("alarm event\r\n");
}
void overflow_event()
{
    uart1.printf("overflow event\r\n");
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("uart ok\r\n");
    
    if(rtc.begin(1) == EOK)//如果设置成功，则使用外部晶振，如果设置失败则使用内部RC震荡器    
        uart1.printf("LSE\r\n");
    else
        uart1.printf("LEI\r\n");
    
    rtc.set_counter(0xfffffffa);
    rtc.set_alarm(5);
    rtc.attach_sec_interrupt(rtcsecit);
    rtc.attach_alarm_interrupt(alarm_event);
    rtc.attach_overflow_interrupt(overflow_event);
    
    rtc.sec_interrupt(ENABLE);
    rtc.alarm_interrupt(ENABLE);
    rtc.overflow_interrupt(ENABLE);
}

int main(void)
{
    setup();
    while(1)
    {
        delay_ms(1000);
    }
}




