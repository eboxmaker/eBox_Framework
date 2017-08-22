/*
file   : *.cpp
author : shentq
version: V1.1
date   : 2016/03/26

Copyright 2016 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "heap.h"
#include "wstring.h"


void mem_test()
{
    String str1;
    String str2;
    char *ptr;
    
    ptr = (char *)mem_malloc(128,1);
    
    str1 = "123";
    str2 = "456\r\n";
    str1 += str2;
    
    memcpy(ptr,str1.c_str(),8);
    
    uart1.printf("ptr point= %d\r\n",ptr);
    uart1.printf("str1 = %s\r\n",str1.c_str());
    uart1.printf_length(ptr,8);
    uart1.printf("free size = %d\r\n",mem_get_free_size(1));
    mem_free(ptr,1);
    uart1.printf("ptr point = %d\r\n",ptr);
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

    mem_test();
    while(1)
    {     				
				
		delay_ms(1000);
    }

}


