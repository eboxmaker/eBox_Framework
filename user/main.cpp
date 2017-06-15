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
#include "tingATCmd.h"

Ting ting;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    ting.begin(&PA1,&uart3,115200);
    uart1.printf("start\r\n");
}
uint16_t addr1;
uint16_t addr;
int main(void)
{
    setup();
    while(1)
    {
        //ting.test();
       /// delay_ms(100);
        //ting.set_addr(++addr1);
        //uart1.printf("addr:%d\r\n",addr1);
        ting.get_addr(&addr);
        uart1.printf("addr:%d\r\n",addr);
        delay_ms(500);
    }

}


