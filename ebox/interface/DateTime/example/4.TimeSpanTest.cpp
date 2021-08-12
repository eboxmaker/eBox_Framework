/**
  ******************************************************************************
  * @file    DateTiemTest.cpp
  * @author  shentq
  * @version V2.0
  * @date    2021/07/23
  * @brief   ebox application example .
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
#include "bsp_ebox.h"
#include "datetime.h"


DateTime dt("2021-07-24 2:28:3");
DateTime dt2("2000-02-3 5:53:27");

//TimeSpan≤‚ ‘
void test()
{
    dt.print(UART);
    dt2.print(UART);

    UART.println("=====TimeSpan 1 Test=====");
    TimeSpan ts;
    ts    = dt2 - dt;
    ts.print(UART);
    UART.println("=====TimeSpan 2 Test=====");
    ts    = dt - dt2;
    ts.print(UART);
    UART.println("=====TimeSpan 3 Test=====");
    DateTime dt3 = dt2 + ts;
    dt3.print(UART);

    UART.println("==========");
    TimeSpan ts0(100,200,313);
    ts0.print(uart1);
    
    UART.println("==========");
    TimeSpan ts1(372313);
    ts1.print(uart1);
    
    UART.println("==========");
    TimeSpan ts2(360000,7,25,13);
    ts2.print(uart1);
    
        
    UART.println("==========");
    TimeSpan ts3(-100,-200,-313);
    ts3.print(uart1);
    
    UART.println("==========");
    TimeSpan ts4(-372313);
    ts4.print(uart1);
    
    UART.println("==========");
    TimeSpan ts5(-3600000,-7,-25,-13);
    ts5.print(uart1);

    UART.println("=====TimeSpan  Test end=====");


    
    
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log();
    test();
}
int main(void)
{

    setup();

    while(1)
    {
        delay_ms(1000);
    }
}




