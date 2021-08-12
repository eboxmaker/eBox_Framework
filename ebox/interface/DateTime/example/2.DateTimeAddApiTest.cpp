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
//时间直接加减测试
void test()
{
    dt.print(UART);

    UART.println("=====DateTime Add Days Test=====");
    for(int i = 0; i < 367; i++)
    {
        dt.addDays(1);
        dt.print(UART);
        UART.flush();
    }
    UART.println("=====DateTime sub Seconds Test=====");
    for(int i = 0; i < 367; i++)
    {
        dt.addSeconds(-3601);
        dt.print(UART);
        UART.flush();
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
        dt.addSeconds(1);
        dt.print(UART);
        UART.println(dt.getTimeStamp());
        delay_ms(1000);
    }
}




