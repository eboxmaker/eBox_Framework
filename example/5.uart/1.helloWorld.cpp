/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */

#include "ebox.h"
void setup()
{
    ebox_init();
    uart1.begin(9600);
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




