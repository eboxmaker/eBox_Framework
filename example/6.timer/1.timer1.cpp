/**
  ******************************************************************************
  * @file    .cpp
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


/* Includes ------------------------------------------------------------------*/

#include "ebox.h"


Timer timer1(TIM1);

void t2it()
{
    PB8.toggle();  
}
class Test 
{
    public:
    void event() 
    {
        PB8.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);

    timer1.begin(1);
    //timer1.attach(t2it);
    timer1.attach(&test,&Test::event);
    timer1.interrupt(ENABLE);
    timer1.start();
    uart1.printf("\r\ntimer clock       = %dMhz", timer1.get_timer_source_clock()/1000000);
    uart1.printf("\r\nmax interrupt frq = %dKhz", timer1.get_max_frq()/1000);
}


int main(void)
{
    setup();
    while(1)
    {

    }


}








