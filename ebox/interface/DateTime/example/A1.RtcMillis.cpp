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
#include "basicRtc.h"

DateTime dt(__DATE__,__TIME__,8);//¶«°ËÇø

RtcMillis rtc;
void setup()
{
    ebox_init();
    UART.begin(115200);
    rtc.begin(dt);
    dt.print(UART);
    rtc.dateTime.print(UART);

}
int main(void)
{

    setup();
    while(1)
    {
        uint32_t last1 = micros();
        rtc.update();
        uint32_t last2 = micros();
        UART.printf("time cost:%dus\n", last2 - last1);
        rtc.dateTime.print(UART);
        delay_ms(100);
    }
}




