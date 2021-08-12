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


DateTimeClass dt1("2021-07-24 0:0:0",8);//东八区
DateTimeClass dt2("2021-07-24 0:0:0",-8);//西八区
DateTimeClass dt3(__DATE__,__TIME__,-8);//西八区

void setup()
{
    ebox_init();
    UART.begin(115200);
    dt1.print(UART);
    dt2.print(UART);
    dt3.print(UART);
    
}
int main(void)
{

    setup();
    while(1)
    {
        delay_ms(1000);
    }
}




