/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


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
    
    rtc.begin();

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




