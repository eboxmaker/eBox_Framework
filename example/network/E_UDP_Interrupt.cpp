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
#include "udp.h"

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
  u8 lip[4]={192,168,1,111};/*定义lp变量*/
  u8 sub[4]={255,255,255,0};/*定义subnet变量*/
  u8 gw[4]={192,168,1,1};/*定义gateway变量*/
	
  u8 rip[4]={192,168,1,102};/*定义lp变量*/
  u8 ip[6];
	
	u8 recvBuf[2048];
W5500 w5500(PA4,SPI1,PA5,PA6,PA7,PA2,PA3);
	
UDP udp;

u16 len;
	
	
void ethEvent()
{

	uart1.printf("\r\n==========");
	uart1.printf("\r\n接收中断！");
	udp.recvFlag = 1;

}
void setup()
{
	eBoxInit();
	uart1.begin(9600);

	w5500.begin(2,mac,lip,sub,gw);
	w5500.attchInterruputEvent(ethEvent);
	attachEthToSocket(&w5500);
	
  w5500.getMAC (ip);
  uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0],ip[1],ip[2],ip[3],ip[4],ip[5]);
  w5500.getIP (ip);
  uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  w5500.getSubnet(ip);
  uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  w5500.getGateway(ip);
  uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  uart1.printf("Network is ready.\r\n");
	
	if(udp.begin(0,30000) == 0)
		uart1.printf("\r\nudp1 server creat ok! listen on 30000");
	

}
int main(void)
{
	setup();

	while(1)
	{
		
		if(udp.recv(recvBuf))
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n本地端口:%d",udp.localPort );
			uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp.remoteIP[0],udp.remoteIP[1],udp.remoteIP[2],udp.remoteIP[3],udp.remotePort);
			uart1.printf("\r\n数据长度:%d",len);		
			uart1.printf("\r\n数据内容:");		
			uart1.printf((const char *)recvBuf);
			udp.sendto(udp.remoteIP,udp.remotePort,recvBuf,100);
		}
//		udp.sendto(rip,8080,data,60);
//		delay_ms(500);




		

	}


}




