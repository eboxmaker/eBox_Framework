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

unsigned char ch;
uint8_t len;

void setup()
{
    ebox_init();
    uart1.begin(115200);
}

int main(void)
{
    setup();
    uint32_t last = millis();
    while(1)
    {
        if(millis() - last > 1000)
        {
            ebox_printf("uart test\r\n");
            last = millis();
        }
//        uart1.printf("hello World !\r\n");
//        delay_ms(1000);
        
        
        len = ebox_fifo_get(uart_fifo_ptr,&ch,1);
        if(len == 1)
        {
            uart1.write(ch);
        }

    }
}




