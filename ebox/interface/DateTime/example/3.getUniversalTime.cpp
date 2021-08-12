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
//获取格林威治时间
void test()
{
    dt.print(UART);

    UART.println("=====DateTime UTC DateTime Test=====\n");
    DateTime dtutc = dt.getUniversalTime();
    dtutc.print(UART);
    UART.println("=====DateTime UTC DateTime Test end=====\n");

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




