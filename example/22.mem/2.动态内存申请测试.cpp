/*
file   : *.cpp
author : shentq
version: V1.1
date   : 2016/03/26

Copyright 2016 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ebox_mem.h"
#include "wstring.h"
#include "bsp_ebox.h"
/**
	*	1	动态内存申请，释放例程
	*/
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"动态内存申请，释放例程"
#define EXAMPLE_DATE	"2018-08-06"



void mem_test()
{
    String str1;
    String str2;
    str1 = "123";
    str2 = "456";
    str1 += str2;
    uart1.printf("str1 = %s\r\n", str1.c_str());

    char *ptr;

    uart1.printf("系统内存剩余 = %dByte\r\n", ebox_get_free());
    uart1.printf("ptr准备申请128字节\r\n");
    ptr = (char *)ebox_malloc(128);
    uart1.printf("ptr实际申请= %d\r\n", ebox_get_sizeof_ptr(ptr));
    uart1.printf("ptr指向= 0x%x\r\n", ptr);
    memcpy(ptr, str1.c_str(), 9);
    uart1.printf("ptr内容：[%s]\r\n", ptr);
    uart1.printf("系统内存剩余 = %dByte\r\n", ebox_get_free());
    ebox_free(ptr);
    uart1.printf("ptr释放\r\n");
    uart1.printf("ptr占用字节= %d\r\n", ebox_get_sizeof_ptr(ptr));
    uart1.printf("系统内存剩余 = %dByte\r\n", ebox_get_free());


}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
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


