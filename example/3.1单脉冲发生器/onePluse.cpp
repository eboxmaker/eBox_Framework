/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"

Pulse pulse(&PA1);

void ajust_ahead_phase()
{
    static int percent = 0;
    
    percent++;
    percent %= 1000;
    pulse.set_phase_front(percent);
    pulse.start();
    delay_ms(10);
    
}
void ajust_rear_phase()
{
    static int percent = 0;
    
    percent++;
    percent %= 1000;

    pulse.set_phase_after(percent);
    pulse.start();
    delay_ms(10);
}
void setup()
{
    ebox_init();
    pulse.cfg.prescaler = 36;//分频
    pulse.cfg.max_counter = 35000;//结束时的counter
    pulse.cfg.delay_counter = 1;//启动时的counter
    pulse.begin(0);
    
}
int main(void)
{
    setup();
    while(1)
    {
        ajust_ahead_phase();
    }
}




