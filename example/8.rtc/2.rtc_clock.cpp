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
RtcClock clock;

void rtcsecit()
{
   clock.sec_event();
   uart1.printf("time=%02d:%02d:%02d\r\n", clock.hour, clock.min, clock.sec);

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
    
    rtc.begin(1);
    clock.set_clock(23,59,55);
    rtc.attach_sec_interrupt(rtcsecit);

    
    rtc.sec_interrupt(ENABLE);

}


int main(void)
{
    setup();

    while(1)
    {
        delay_ms(1000);
    }
}




