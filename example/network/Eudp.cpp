
#include "ebox.h"
#include "w5500.h"
#include "socket.h"
#include "udp.h"
USART uart1(USART1,PA9,PA10);

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
  u8 lip[4]={192,168,1,111};/*定义lp变量*/
  u8 sub[4]={255,255,255,0};/*定义subnet变量*/
  u8 gw[4]={192,168,1,1};/*定义gateway变量*/
	
  u8 rip[4]={192,168,1,102};/*定义lp变量*/
	u8 data[20] = "\r\n data";
  u8 ip[6];
	
W5500 w5500(PA4,SPI1,PA5,PA6,PA7,PA2,PA3);
	
UDPSERVER udp;
UDPSERVER udp2;
UDPCLIENT udpc;
	
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	
	w5500.begin(mac,lip,sub,gw);

	attachEth(&w5500);
	
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
		uart1.printf("\r\nudp server creat ok! listen on 30000");
	if(udp2.begin(1,30001) == 0)
		uart1.printf("\r\nudp server creat ok! listen on 30001");

	udpc.begin(2,30002);

}

u8 ret;
u8 process()
{
	u8 ret = 0;
	
	if(eth->getSn_IR(0))
	{
		udp.process();
		ret = 1;
	}
	if(eth->getSn_IR(1))
	{
		udp2.process();
		ret = 2;
	}
	if(eth->getSn_IR(2))
	{
		udpc.process();
		ret = 3;
	}
	return ret;
}
u16 len;
int main(void)
{
	setup();

	while(1)
	{
		ret = process();
		if(ret == 1)
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n本地端口:%d",udp.localPort );
			uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp.remoteIP[0],udp.remoteIP[1],udp.remoteIP[2],udp.remoteIP[3],udp.remotePort);
			uart1.printf("\r\n数据内容:");		
			uart1.printf((const char *)udp.buf);		
		}
		if(ret == 2)
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n本地端口:%d",udp2.localPort );
			uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp2.remoteIP[0],udp2.remoteIP[1],udp2.remoteIP[2],udp2.remoteIP[3],udp2.remotePort);
			uart1.printf("\r\n数据内容:");		
			uart1.printf((const char *)udp2.buf);		

		}
		len = udpc.send(rip,8080,data,20);
		uart1.printf("\r\ndata:");
		uart1.printf((const char  *)data);
		uart1.printf("\r\nudpc send %d len data",len);
		delay_ms(500);

		

	}


}




