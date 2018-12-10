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
#include "../edriver/wifi/ESP8266.h"


/**
	*	1	此例程演示ESP8266的初始化和版本信息打印
	*	2	此例需要在工程中添加ESP8266.cpp,esp8266_upd.cpp,esp8266_tcp.cpp
	*/
/* 定义例程名和例程发布日期 */


#define EXAMPLE_NAME	"esp8266 basic test example"
#define EXAMPLE_DATE	"2018-08-11"

char recv_buf[1024] = {0};


void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    wifi.begin(&PA4, &uart2, 115200);

}

int main(void)
{
    bool ret;
    setup();


    ret = wifi.kick();
    if(ret)
        uart1.printf("kick ok\r\n");
    else
        uart1.printf("kick failed\r\n");

    ret = wifi.get_version(recv_buf);
    if(ret)
        uart1.printf("%s\r\n", recv_buf);
    else
        uart1.printf("get version failed\r\n");
    while(1)
    {



    }

}





