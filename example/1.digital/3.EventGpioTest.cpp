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
#include "EventGpio.h"

void neg()
{
    uart1.println("neg");
}
void pos()
{
    uart1.println("pos");
}
void high()
{
    uart1.println("high");

}
void click()
{
    uart1.println("click");
}
void release()
{
    uart1.println("release");
}
void long_press()
{
    uart1.println("long");
}
void click1()
{
    uart1.println("click1");
}
void release1()
{
    uart1.println("release1");
}
void long_press1()
{
    uart1.println("long1");
}
EventGpio fungpio8(&PA8,0,0,0,0,click,release,0);
EventGpio fungpio0(&PB1,0,0,0,0,click1,release1,long_press1);
FunHal axb;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    fungpio8.begin(1);
    fungpio0.begin(1);
    axb.add(&fungpio0);
    axb.add(&fungpio8);
}
int main(void)
{
    setup();

    while(1)
    {
        axb.process();
        delay_ms(1);
    }
}



