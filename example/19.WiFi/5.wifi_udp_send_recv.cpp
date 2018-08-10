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
#include "../edriver/wifi/esp8266_udp.h"


/**
	*	1	��������ʾudpʹ�÷���
	*	2	������Ҫ�ڹ��������ESP8266.cpp,esp8266_upd.cpp,esp8266_tcp.cpp
	*/
/* ���������������̷������� */
#define EXAMPLE_NAME	"esp8266 upd example"
#define EXAMPLE_DATE	"2018-08-11"



WIFI_UDP udp(&wifi);


char        remote_ip[] = "192.168.1.101";
uint16_t    remote_port = 8080;
uint16_t    local_port = 4321;


char recv_buf[1024] = {0};
uint8_t send_buf[] = "this is a udp send test!\r\n";
uint16_t len = 0;
uint32_t count = 0;

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
    ret = udp.begin();
    if(ret)
		uart1.printf("esp8266 upd OK\r\n");

}

int main(void)
{
    bool ret;
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

    //upd connect to another 
    ret = udp.connect(remote_ip, remote_port, local_port);
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
        len = udp.read((uint8_t *)recv_buf);
        if(len)
        {

            uart1.write((char *)recv_buf, len);

        }
        if(count == 0)
        {
            ret = udp.send((uint8_t *)send_buf, sizeof(send_buf));
            if(ret)
                uart1.printf("send ok!\r\n");
        }
        count++;
        count %= 500000;

    }



}





