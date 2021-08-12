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
#include "datetime.h"


DateTimeClass dt("2021-07-24 0:0:0",8);//东八区
DateTimeClass dt2("2021-07-24 0:0:0",-8);//西八区

void setup()
{
    ebox_init();
 
    uart1.begin(115200);
    uart3.begin(115200);
    
}
int main(void)
{

    setup();
    
    TimeSpan ts(100,200,313);
    ts.print(uart1);
    
    TimeSpan ts1(372313);
    ts1.print(uart1);
    
    TimeSpan ts2(360000,7,25,13);
    ts2.print(uart1);
    
        
    TimeSpan ts3(-100,-200,-313);
    ts3.print(uart1);
    
    TimeSpan ts4(-372313);
    ts4.print(uart1);
    
    TimeSpan ts5(-3600000,-7,-25,-13);
    ts5.print(uart1);

    TimeSpan ts6 = dt - dt2;
    ts6.print(uart1);


    while(1)
    {
        delay_ms(1000);
    }
}




