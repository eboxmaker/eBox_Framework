/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ESP8266.h"
#include "esp8266_udp.h"


WIFI_UDP udp(&wifi);


char recv_buf[1024] = {0};
uint16_t len = 0;
bool ret;


void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("esp8266 join ap test\r\n");
    uart1.printf("--------------\r\n");

    ret = wifi.begin(&PA4, &uart2, 115200);
    if(ret)
        uart1.printf("esp8266 AT_CMD OK\r\n");
    
	uart1.printf("esp8266 join wifi...\r\n");
    ret = wifi.join_ap();
    if(ret)
		uart1.printf("esp8266 join wifi OK\r\n");
		

}

int main(void)
{
    setup();

	uart1.printf("esp8266 get ipconfig ...\r\n");
    ret = wifi.query_sta_ip(recv_buf);
    if(ret)
    {
        uart1.printf("%s\r\n", recv_buf);
    }
    ret = wifi.query_sta_gateway(recv_buf);
    if(ret)
    {
        uart1.printf("%s\r\n", recv_buf);
    }
    ret = wifi.query_sta_netmask(recv_buf);
    if(ret)
    {
        uart1.printf("%s\r\n", recv_buf);
    }


    while(1);

}