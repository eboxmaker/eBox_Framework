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


DateTimeClass dt("2021-01-2 2:28:3");
//DateTimeClass dt2("2022-02-3 3:54:50");
DateTimeClass dt2("2000-02-3 5:53:27");

void setup()
{
    ebox_init();
    uart1.begin(115200);
}
int main(void)
{

    setup();
    
    dt.print(uart1);
    dt2.print(uart1);
    
    uart1.println("=====DateTime Add Days Test=====");
    for(int i = 0; i < 367; i++)
    {
        dt.add_days(1);
        dt.print(uart1);
        uart1.flush();
    }
    uart1.println("=====DateTime Add Seconds Test=====");
    for(int i = 0; i < 367; i++)
    {
        dt.add_seconds(3601);
        dt.print(uart1);
        uart1.flush();
    }
    uart1.println(dt.toString());
    uart1.printf("day of year = %d\n",dt2.day_of_year());

    uart1.println("=====TimeSpan 1 Test=====");
    TimeSpan ts;
    ts    = dt2 - dt;
    ts.print(uart1);
    uart1.println("=====TimeSpan 2 Test=====");
    ts    = dt - dt2;
    ts.print(uart1);
    
    DateTimeClass dt3 = dt2 + ts;
    

    dt3.print(uart1);
    dt2.print(uart1);
    
    uart1.println("=====DateTime Formate Print Test=====");

    for(int i = 0; i < 8;i++)
    {
        for(int j = 0;j<3;j++)
        {
            uart1.println(dt.toString((DateTimeClass::TimeFormat_t)i,(DateTimeClass::TimeSeparatorFormat_t)j));

        }
    }
    uart1.println("=====DateTime Run Test=====");

    while(1)
    {
        dt.add_seconds(1);
        dt.print(uart1);
        delay_ms(1000);
    }
}




