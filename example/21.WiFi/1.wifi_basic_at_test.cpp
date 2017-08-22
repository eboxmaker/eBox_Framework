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

char    buf[1024] = {0};

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("esp8266 basic at cmd test\r\n");
    uart1.printf("--------------\r\n");

    wifi.begin(&PA4, &uart2, 115200);

}


uint8_t mode;
bool ret;
int main(void)
{
    setup();

    ret = wifi.kick();
    if(ret)
        uart1.printf("kick ok\r\n");
    else
        uart1.printf("kick failed\r\n");

    ret = wifi.get_version(buf);
    if(ret)
        uart1.printf("%s\r\n", buf);
    else
        uart1.printf("get version failed\r\n");

    while(1);

}