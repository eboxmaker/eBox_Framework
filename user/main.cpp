/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "w5500.h"
#include "tcp.h"

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
  u8 lip[4]={192,168,1,119};/*定义lp变量*/
  u8 sub[4]={255,255,255,0};/*定义subnet变量*/
  u8 gw[4]={192,168,1,1};/*定义gateway变量*/
	u8 dns[4] = {192,168,1,1};
  u8 rip[4]={192,168,1,102};/*定义lp变量*/
	u8 buf[1024];
  u8 ip[6];
	u16 len;
	
CONFIG_MSG ConfigMsg;

	
W5500 w5500(&PC13,&PC14,&PC15,&spi2);
	
TCPSERVER tcpServer;

int ret;

	#include "string.h"

void setup()
{
	ebox_init();
	uart1.begin(9600);
	ret = sizeof(long);
	ret = sizeof(int);
	ret = sizeof(short int);
	ret = sizeof(short);

	w5500.begin(2,mac,lip,sub,gw);

	
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
	
	memcpy(ConfigMsg.lip, lip, 4);
  memcpy(ConfigMsg.sub, sub, 4);
  memcpy(ConfigMsg.gw,  gw, 4);
  memcpy(ConfigMsg.mac, mac,6);
  memcpy(ConfigMsg.dns,dns,4);
  sprintf((char*)ConfigMsg.domain,"%s","www.baidu.com"); 
//	ret = tcpServer.begin(SOCKET0,30000);
//	if(ret == 0)
//		uart1.printf("\r\n success !");

}
#define  DNS_RETRY            100//3 times

#include "string.h"
#include "dns.h"
#define SOCK_DNS              2
int main(void)
{
	  int8_t dns_retry_cnt=0;
  int8_t dns_ok=0;

	setup();

	while(1)
	{
    if( (dns_ok==1) ||  (dns_retry_cnt > DNS_RETRY))
    {
      uart1.printf("dns ok.\r\n");
						while(1);

    }
    else if(memcmp(ConfigMsg.dns,"\x00\x00\x00\x00",4))
    {
      switch(dns_query(SOCK_DNS,ConfigMsg.domain)) /*发送DNS请求*/
      {
        case DNS_RET_SUCCESS:
          dns_ok=1;
          memcpy(ConfigMsg.rip,DNS_GET_IP,4);
          dns_retry_cnt=0;
          uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d] from %d.%d.%d.%d\r\n",ConfigMsg.domain,ConfigMsg.rip[0],ConfigMsg.rip[1],ConfigMsg.rip[2],ConfigMsg.rip[3],ConfigMsg.dns[0],ConfigMsg.dns[1],ConfigMsg.dns[2],ConfigMsg.dns[3]);
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
  else
     uart1.printf("Invalid DNS server [%d.%d.%d.%d]\r\n",ConfigMsg.dns[0],ConfigMsg.dns[1],ConfigMsg.dns[2],ConfigMsg.dns[3]);



		

	}


}




