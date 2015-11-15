/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "string.h"
#include "ebox.h"
#include "w5500.h"
#include "tcp.h"
#include "dns.h"

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
  u8 lip[4]={192,168,1,199};/*定义lp变量*/
  u8 sub[4]={255,255,255,0};/*定义subnet变量*/
  u8 gw[4]={192,168,1,1};/*定义gateway变量*/
	u8 dns[4] = {192,168,1,1};
  u8 rip[4]={192,168,1,102};/*定义lp变量*/
	u8 buf[1024];
  u8 ip[6];
	u16 len;
	


DNS ddns;

u8 name[]="www.nciae.edu.cn";

	
	

	
W5500 w5500(&PC13,&PC14,&PC15,&spi2);
	

int ret;

	#include "string.h"

void setup()
{
	ebox_init();
	uart1.begin(115200);
	ret = sizeof(long);
	ret = sizeof(int);
	ret = sizeof(short int);
	ret = sizeof(short);

	w5500.begin(2,mac,lip,sub,gw,dns);

	
	attach_eth_to_socket(&w5500);
	
  w5500.getMAC (ip);
  uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0],ip[1],ip[2],ip[3],ip[4],ip[5]);
  w5500.getIP (ip);
  uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  w5500.getSubnet(ip);
  uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  w5500.getGateway(ip);
  uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  uart1.printf("Network is ready.\r\n");
	
	ddns.begin(SOCKET1,3000);



}

int main(void)
{

  uint8_t dns_retry_cnt=0;
  uint8_t dns_ok=0;
	setup();

	while(1)
	{
      switch(ddns.query(name)) /*发送DNS请求*/
      {
        case DNS_RET_SUCCESS:
          dns_ok=1;
          dns_retry_cnt=0;
          uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d] from %d.%d.%d.%d\r\n",name,ddns.domain_ip[0],ddns.domain_ip[1],ddns.domain_ip[2],ddns.domain_ip[3],dns[0],dns[1],dns[2],dns[3]);
          break;
        case DNS_RET_FAIL:
          dns_ok=0;
          dns_retry_cnt++;
          uart1.printf("Fail! Please check your network configuration or DNS server.\r\n");
          break;
        default:
          break;
      }
    



		

	}


}




