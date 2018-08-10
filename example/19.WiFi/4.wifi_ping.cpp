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
	*	1	��������ʾpingʹ�÷���
	*	2	������Ҫ�ڹ��������ESP8266.cpp,esp8266_upd.cpp,esp8266_tcp.cpp
	*/
/* ���������������̷������� */
#define EXAMPLE_NAME	"esp8266 ping example"
#define EXAMPLE_DATE	"2018-08-11"

char recv_buf[1024] = {0};

bool ret;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    wifi.begin(&PA4, &uart2, 115200);
    wifi.get_ap_list((char *)recv_buf);
    uart1.printf((char *)recv_buf);
    ret = wifi.join_ap();
    if(ret)
		uart1.printf("esp8266 join wifi OK\r\n");
}

int main(void)
{
    bool ret;
    setup();


    while(1)
    {
        wifi.ping("www.baidu.com",recv_buf);
            uart1.printf(recv_buf);
    }

}





