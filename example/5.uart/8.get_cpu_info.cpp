
/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V2.2
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
    uart1.begin(115200,0);
    
    uart1.printf("core:%d\r\n",cpu.clock.core);
    uart1.printf("core:%d\r\n",cpu.clock.core);
    uart1.printf("hclk:%d\r\n",cpu.clock.hclk);
    uart1.printf("pclk1:%d\r\n",cpu.clock.pclk1);
    uart1.printf("pclk2:%d\r\n",cpu.clock.pclk2);
    uart1.printf("cpu id:0x%x %x %x\r\n",cpu.chip_id[0],cpu.chip_id[1],cpu.chip_id[2]);
    uart1.printf("size:%dK\r\n",cpu.flash_size);
    uart1.printf("company:%s\r\n",cpu.company);
    uart1.printf("ability:%0.2fM times add per second\r\n",cpu.ability/1000000.0);
}

int main(void)
{
    setup();

    while(1)
    {

        delay_ms(1000);
    }

}







