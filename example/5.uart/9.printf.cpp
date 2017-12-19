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
#include "printx.h"
#include "fifo.h"
void setup()
{
    ebox_init();

    uart_fifo_ptr = my_fifo_alloc(200);
 
    uart1.begin(115200);
}
uint8_t buf[200];
uint8_t len;
uint8_t count = 0;
int main(void)
{
    setup();
    while(1)
    {
        count ++;
        ebox_printf("%d:qqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqqq\r\n",count);
        
        len = my_fifo_get(uart_fifo_ptr,buf,200);
        if(len > 0 )
            uart1.write(buf,len);
    }
}




