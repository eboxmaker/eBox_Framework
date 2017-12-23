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
                                            
uint8_t *ptr;
void setup()
{
    ebox_init();
    uart1.begin(115200);
    
    uart1.printf("\r\n");
    uart1.printf("----------------mem test--------------------\r\n");

    uart1.printf("mem :0x%x---0x%x(%dK%dbytes)\r\n",ebox_get_sram_start_addr(),ebox_get_sram_end_addr(),(ebox_get_free())/1024,(ebox_get_free())%1024);
 
    
    uart1.printf("-------------------------------------------\r\n");
    uart1.printf("mem free before:0X%Xbytes\r\n",ebox_get_free());
    ptr = (uint8_t *)ebox_malloc(1000);
    uart1.printf("mem free after :0X%Xbytes\r\n",ebox_get_free());
    ebox_free(ptr);
    uart1.printf("mem free end   :0X%Xbytes\r\n",ebox_get_free());
    uart1.printf("-------------------------------------------\r\n");
    uart1.printf("mem free before:0X%Xbytes\r\n",ebox_get_free());
    ptr = (uint8_t *)ebox_malloc(123);
    uart1.printf("mem free after :0X%Xbytes\r\n",ebox_get_free());
    ebox_free(ptr);
    uart1.printf("mem free end   :0X%Xbytes\r\n",ebox_get_free());
    
}
int main(void)
{
    setup();
    while(1)
    {
        delay_ms(1000);
    }

}


