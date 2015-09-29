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

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*����Mac����*/
  u8 lip[4]={192,168,1,111};/*����lp����*/
  u8 sub[4]={255,255,255,0};/*����subnet����*/
  u8 gw[4]={192,168,1,1};/*����gateway����*/
	
  u8 rip[4]={192,168,1,102};/*����lp����*/
	u8 buf[1024];
  u8 ip[6];
	u16 len;

W5500 w5500(&PC13,&PC14,&PC15,&spi2);
	
TCPSERVER tcpServer;

int ret;


void setup()
{
	eBoxInit();
	uart1.begin(9600);

	w5500.begin(2,mac,lip,sub,gw);

	
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
	
	ret = tcpServer.begin(SOCKET0,30000);
//	if(ret == 0)
//		uart1.printf("\r\n success !");

}
int main(void)
{
	setup();

	while(1)
	{
		len = tcpServer.recv(buf);
		if(len > 0)
		{
			uart1.printf("\r\n==================");
			uart1.printf("\r\n�ͻ���:%d.%d.%d.%d:%d",tcpServer.remoteIP[0],tcpServer.remoteIP[1],tcpServer.remoteIP[2],tcpServer.remoteIP[3],tcpServer.remotePort);
			uart1.printf("\r\n���ݳ��ȣ�%d",len);
			uart1.printf("\r\n�������ݣ�");
			uart1.printf((const char*)buf);
			tcpServer.send(buf,len);
		}
		

	}


}




