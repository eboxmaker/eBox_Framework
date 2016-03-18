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
#include "esp8266_tcp.h"


WIFI_TCP tcp(&wifi);


char        remote_ip[] = "192.168.1.101";
uint16_t    remote_port = 8080;
uint16_t    local_port = 4321;


uint8_t recv_buf[1024] = {0};
uint8_t send_buf[] = "this is a udp send test!\r\n";
uint16_t len = 0;
uint32_t count = 0;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("esp8266 tcp single client test\r\n");
    uart1.printf("-------------------------------\r\n");

    wifi.begin(&PA4, &uart2, 115200);
    wifi.join_ap();
    tcp.begin();

}

int main(void)
{
    bool ret;
    setup();

    ret = tcp.connect(remote_ip, remote_port, local_port);
    if(ret)
    {
        uart1.printf("connect ok!\r\n");
    }
    else
    {
        uart1.printf("connect failed!\r\n");
		
    }

    while(1)
    {
        len = tcp.read((uint8_t *)recv_buf);
        if(len)
        {

            uart1.printf_length((char *)recv_buf, len);

        }
        if(count == 0)
        {
            ret = tcp.send((uint8_t *)send_buf, sizeof(send_buf));
            if(ret)
                uart1.printf("send ok!\r\n");
        }
        count++;
        count %= 500000;

    }

}





