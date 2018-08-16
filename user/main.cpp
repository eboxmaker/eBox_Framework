/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "bsp_ebox.h"

#include "myprintf.h"

/**
	*	1	此例程需要调用eDrive目录下的w5500模块
	*	2	此例程演示了w5500的初始化，基本信息打印
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w5500 io test example"
#define EXAMPLE_DATE	"2018-08-11"

char buf[100];

void vsn_test(const char *fmt, ...)
{
    int  size1 = 0;
    size_t  size2 = 100;
    va_list va_params;

    va_start(va_params, fmt);

//    do{
//        
//        buf = (char *)ebox_malloc(size2);
//        if(buf == NULL)
//            return ;
        size1 = MyVsnprintf(buf,size2,fmt, va_params);
//        if(size1 == -1  || size1 >= size2)
//        {
//            size2+=32;
//            size1 = -1;
//            ebox_free(buf);
//        }
//    }while(size1 == -1);

    va_end(va_params);

//    uart1.println(size1);
    uart1.printf(buf);
    
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    
    
//    for (int i = 0; i < 30;i++)
//    {
//        uart1.printf("%lld\r\n",mypow10(i));
//    }
//    uart1.printf("e:%lld\r\n",(unsigned long long)1e+18);
//    
//    float x=PI;
//    for (int i = 0; i < 20;i++)
//    {
//        uart1.printf("%f,%d\r\n",x,get_float_integer_len(x));
//        x*=10;
//    }

//   uart1.printf("%s%0.1f\r\n","abcdefghij",12.36);
//   uart1.printf("%f\r\n",12.345678);
//   vsn_test("%f\r\n",12.345678);
//    
//   uart1.printf("%0.1f\r\n",12.345678);
//   vsn_test("%0.1f\r\n",12.345678);
//    
//   uart1.printf("%2.1f\r\n",12.345678);
//   vsn_test("%2.1f\r\n",12.345678);
//    
//   uart1.printf("%3.1f\r\n",12.345678);
//   vsn_test("%3.1f\r\n",12.345678);
//    
//   uart1.printf("%3.2f\r\n",12.345678);
//   vsn_test("%3.2f\r\n",12.345678);
//    
//   uart1.printf("%3.2f\r\n",12.345678);
//   vsn_test("%3.2f\r\n",12.345678);
//    
//   uart1.printf("%4.4f\r\n",12.345678);
//   vsn_test("%4.4f\r\n",12.345678);
//    
//   uart1.printf("%5.3f\r\n",12.345678);
//   vsn_test("%5.3f\r\n",12.345678);
//    
//   uart1.printf("%010.5f\r\n",12.345678);
//   vsn_test("%010.5f\r\n",12.345678);
    
//    vsn_test("d:\t|%5d\r\n",123);
//    uart1.printf("d:\t|%d\r\n",123);

//    vsn_test("1d:\t|%1d\r\n",123);
//    uart1.printf("1d:\t|%1d\r\n",123);
//    
//    vsn_test("2d:\t|%2d\r\n",123);
//    uart1.printf("2d:\t|%2d\r\n",123);
//    
//    vsn_test("3d:\t|%3d\r\n",123);
//    uart1.printf("3d:\t|%3d\r\n",123);
//    
//    vsn_test("4d:\t|%04d\r\n",123);
//    uart1.printf("4d:\t|%04d\r\n",123);
    
//    uart1.printf("%c\r\n",'A');
//    uart1.printf("%3c\r\n",'A');
//    uart1.printf("%-3c\r\n",'A');
//    
//    vsn_test("%c\r\n",'A');
//    vsn_test("%3c\r\n",'A');
//    vsn_test("%-3c\r\n",'A');
    
//    vsn_test("%020.5f\r\n",1234567890.12345678);
//    vsn_test("%-020.5f\r\n",1234567890.12345678);
//    
    uart1.printf("%04d\r\n",123);
    uart1.printf("%-04d\r\n",123);
    vsn_test("%04d\r\n",123);
    vsn_test("%-04d\r\n",123);
    
    uart1.printf("%05o\r\n",123);
    vsn_test("%05o\r\n",123);

//    uart1.print(1234567.345678,3);
//   vsn_test("%10.3f\r\n",12.345678);
//   vsn_test("%10.3f\r\n",12.345678);
   uart1.printf("|%-03S |%20.9f |%d |%c\r\n","abcdefghij",123456789.36777777777777777777777778 , 987,'A');
   vsn_test("|%3s |%20.9f |%d |%c\r\n","abcdefghij",123456789.36777777777777777777777778 , 987,'A');
}
unsigned long sendNTPpacket(char* address);
int main(void)
{
    setup();

    while(1)
    {
    }

}
