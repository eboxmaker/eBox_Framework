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

#define SSID        "DVmaster"
#define PASSWORD    "1dvmaster456"

char remote_ip[]="192.168.1.101";
uint16_t remote_port = 8080;
uint16_t local_port = 4321;


uint8_t recv_buf[1024]={0};
uint16_t len = 0;

char buf1[1024];

void setup()
{
	ebox_init();
	uart1.begin(115200);
	uart1.printf("esp8266 udp single client test\r\n");
	uart1.printf("--------------\r\n");
       
	wifi.begin(&PA4,&uart2,115200);
    wifi.join_ap((char *)SSID,(char *)PASSWORD);
    
}	


uint8_t mode;
bool ret;
int main(void)
{   
	setup();  
	
	ret = wifi.query_sta_ip(buf1);
	if(ret)
	{
			uart1.printf("%s\r\n",buf1);
	}
	ret = wifi.query_sta_gateway(buf1);
	if(ret)
	{
			uart1.printf("%s\r\n",buf1);
	}
	ret = wifi.query_sta_netmask(buf1);
	if(ret)
	{
			uart1.printf("%s\r\n",buf1);
	}


	while(1);

}