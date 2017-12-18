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
#include "EventManager.h"
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

void var_chage()
{
    uart1.print("var changed\r\n");
};
void var_pos()
{
    uart1.print("var pos\r\n");
};
void var_neg()
{
    uart1.print("var neg\r\n");
};
uint8_t var;
EventGpio event_io_2(&PA8,0,0,pos,neg,click1,release1,long_press1);
EventVar ev(&var,0,var_neg,0);
EventManager io_manager;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    event_io_2.begin(1);
//    io_manager.add(&ev);
    io_manager.add(&event_io_2);
}
int main(void)
{
    setup();

    while(1)
    {
        var++;
        io_manager.process();
        delay_ms(1);
    }
}



