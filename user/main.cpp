/*
file   : *.cpp
author : shentq
version: V1.1
date   : 2016/03/26

Copyright 2016 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ultrasonic_wave.h"

#include "ebox_mem.h"
    int i;
void mem_test()
{
u8 *ptr;
u16 *ptr1;
u32 *ptr2;
u32 *ptr3;
    
ptr=(u8*)mymalloc(100);
if(ptr)
{
for(int i = 0; i < 10;i++)
    {
    *ptr =1;
    }
    uart1.printf("i = %d\r\n",ptr[0]);   
    uart1.printf("ptr = %d\r\n",ptr);   
}   
myfree(ptr);
if(ptr == NULL)
uart1.printf("free ptr\r\n");  
else
uart1.printf("ptr = %d\r\n",ptr);   
    uart1.printf("i = %d\r\n",ptr[0]);   



}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("ok \r\n");   
}
int main(void)
{
    setup();


    while(1)
    {       

mem_test();
				
				
		delay_ms(1000);
    }

}


