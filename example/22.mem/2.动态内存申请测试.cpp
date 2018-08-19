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
	*	1	��̬�ڴ����룬�ͷ�����
	*/
/* ���������������̷������� */
#define EXAMPLE_NAME	"��̬�ڴ����룬�ͷ�����"
#define EXAMPLE_DATE	"2018-08-06"



void mem_test()
{
    String str1;
    String str2;
    str1 = "123";
    str2 = "456";
    str1 += str2;  
    uart1.printf("str1 = %s\r\n",str1.c_str());

    char *ptr;
    
    uart1.printf("ϵͳ�ڴ�ʣ�� = %dByte\r\n",ebox_get_free());
    uart1.printf("ptr׼������128�ֽ�\r\n");
    ptr = (char *)ebox_malloc(128);
    uart1.printf("ptrʵ������= %d\r\n",ebox_get_sizeof_ptr(ptr));
    uart1.printf("ptrָ��= 0x%x\r\n",ptr);
    memcpy(ptr,str1.c_str(),9);
    uart1.printf("ptr���ݣ�[%s]\r\n",ptr);
    uart1.printf("ϵͳ�ڴ�ʣ�� = %dByte\r\n",ebox_get_free());
    ebox_free(ptr);
    uart1.printf("ptr�ͷ�\r\n");
    uart1.printf("ptrռ���ֽ�= %d\r\n",ebox_get_sizeof_ptr(ptr));
    uart1.printf("ϵͳ�ڴ�ʣ�� = %dByte\r\n",ebox_get_free());
    
    
}
void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
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


