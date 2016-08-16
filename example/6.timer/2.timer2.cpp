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




uint32_t xx;
uint8_t flag;
Timer timer2(TIM1);

void t2it()
{
    xx++;
    if(xx == 1000)
    {
        flag = 1;
        xx = 0;
        PB8.toggle();
    }
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    PB8.mode(OUTPUT_PP);

    timer2.begin(1000);
    timer2.attach_interrupt(t2it);
    timer2.interrupt(ENABLE);
    timer2.start();
    uart1.printf("\r\ntimer clock       = %dMhz", timer2.get_timer_clock()/1000000);
    uart1.printf("\r\nmax interrupt frq = %dKhz", timer2.get_max_frq()/1000);
}


int main(void)
{
    setup();
    while(1)
    {
        if(flag == 1)
        {
            uart1.printf("\r\ntimer2 is triggered 1000 times !");
            flag = 0;
        }
    }


}








