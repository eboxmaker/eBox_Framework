 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
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

#include "ebox.h"
void setup()
{
    ebox_init();
    uart1.begin(115200);
}
char buf[] = "hello world !\r\n";
int main(void)
{
    setup();
    while(1)
    {
        uart1.printf("hello World !\r\n");
        uart1.printf_length(buf, sizeof(buf));
        delay_ms(1000);
    }
}




