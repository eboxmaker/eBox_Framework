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


DateTime dt(__DATE__,__TIME__,8);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log();
}
int main(void)
{
    setup();
    while(1)
    {
        dt.addSeconds(1);
        dt.print(UART);
        delay_ms(1000);
    }
}




