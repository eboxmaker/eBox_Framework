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
        PB8.write(!PB8.read());
    }
}
void setup()
{
    ebox_init();
    uart1.begin(9600);
    PB8.mode(OUTPUT_PP);

    timer2.begin(1000);
    timer2.attach_interrupt(t2it);
    timer2.interrupt(ENABLE);
    timer2.start();
}


int main(void)
{
    setup();
    while(1)
    {
        if(flag == 1)
        {
            uart1.printf("\r\ntimer2 is triggered 1000 times !", xx);
            flag = 0;
        }
    }


}








