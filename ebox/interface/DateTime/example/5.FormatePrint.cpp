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

//DateTime Formate Print Test
void test()
{
    
    UART.println("=====DateTime Formate Print Test=====");

    for(int i = 0; i < 8;i++)
    {
        for(int j = 0;j<3;j++)
        {
            UART.println(dt.toString((DateTime::TimeFormat_t)i,(DateTime::TimeSeparatorFormat_t)j));

        }
    }
    
    
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




