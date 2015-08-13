
#include "ebox.h"
#include "w5500.h"
#include "udp.h"
USART uart1(USART1,PA9,PA10);

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
  u8 lip[4]={192,168,1,111};/*定义lp变量*/
  u8 sub[4]={255,255,255,0};/*定义subnet变量*/
  u8 gw[4]={192,168,1,1};/*定义gateway变量*/
	
  u8 rip[4]={192,168,1,102};/*定义lp变量*/
	u8 data[64];
  u8 ip[6];
	
W5500 w5500(PA4,SPI1,PA5,PA6,PA7,PA2,PA3);
	
UDPCLIENT udpc;

	u8 recvBuf[2048];
u8 count;
void ethEvent()
{
	u8 tmp2,tmp3;
if(udpc.recvFlag == 0)
{
	tmp2 = eth->getSIR();
	tmp3 = eth->getSn_IR(SOCKET0);
	if(tmp2&SIR_0)
	{
		eth->setSIR(SIR_0);/*SIR的第0位置1*/
	}
	
	if(tmp3&Sn_IR_RECV)
	{
		eth->setSn_IR(SOCKET0, Sn_IR_RECV);/*Sn_IR的第2位置1*/
		udpc.interruptRecv(recvBuf);
				udpc.recvFlag = 1;
	}	

	count++;
	#if 0
	tmp2 = eth->getSIR();
	tmp3 = eth->getSn_IR(SOCKET0);
	uart1.printf("\r\nSIR:0x%02x",tmp2);
	uart1.printf("\r\nSn_IR:0x%02x",tmp3);
	#endif

}


}
void setup()
{
	eBoxInit();
	uart1.begin(9600);

	w5500.begin(mac,lip,sub,gw);
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
	
	udpc.begin(SOCKET0,30000);
	

}
u16 len;
int main(void)
{
	setup();
	for(int i= 0; i < 64; i++)
	 data[i] = 0x30+i%10;
	while(1)
	{
		
		if(udpc.recvFlag == 1)
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n本地端口:%d",udpc.localPort );
			uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udpc.remoteIP[0],udpc.remoteIP[1],udpc.remoteIP[2],udpc.remoteIP[3],udpc.remotePort);
			uart1.printf("\r\n数据长度:%d",len);		
			uart1.printf("\r\n数据内容:");		
			uart1.printf((const char *)recvBuf);
					udpc.recvFlag = 0;

		}
//		udpc.sendto(rip,8080,data,60);
//		delay_ms(500);




		

	}


}




