
#include "ebox.h"
#include "w5500.h"
#include "socket.h"
#include "udp.h"
USART uart1(USART1,PA9,PA10);

  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*����Mac����*/
  u8 lip[4]={192,168,1,111};/*����lp����*/
  u8 sub[4]={255,255,255,0};/*����subnet����*/
  u8 gw[4]={192,168,1,1};/*����gateway����*/
	
  u8 rip[4]={192,168,1,102};/*����lp����*/
	u8 data[20] = "\r\ntest data";
  u8 ip[6];
	
W5500 w5500(PA4,SPI1,PA5,PA6,PA7,PA2,PA3);
	
UDPSERVER udp1;
UDPSERVER udp2;
	
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	uart1.printf("\r\nuart1 9600 ok!");
	
	w5500.begin(mac,lip,sub,gw);

	attachEthToSocket(&w5500);
	
  w5500.getMAC (ip);
  uart1.printf("\r\nmac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0],ip[1],ip[2],ip[3],ip[4],ip[5]);
  w5500.getIP (ip);
  uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  w5500.getSubnet(ip);
  uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  w5500.getGateway(ip);
  uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  uart1.printf("Network is ready.\r\n");
	
	if(udp1.begin(0,30000) == 0)
		uart1.printf("\r\nudp1 server creat ok! listen on 30000");
	if(udp2.begin(1,30001) == 0)
		uart1.printf("\r\nudp2 server creat ok! listen on 30001");

}
u16 len;
int main(void)
{
	setup();

	while(1)
	{

		if(udp1.recv())
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n���ض˿�:%d",udp1.localPort );
			uart1.printf("\r\n��Ϣ��Դ:%d.%d.%d.%d:%d", udp1.remoteIP[0],udp1.remoteIP[1],udp1.remoteIP[2],udp1.remoteIP[3],udp1.remotePort);
			uart1.printf("\r\n��������:");		
			uart1.printf((const char *)udp1.buf);		
		}
		if(udp2.recv())
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n���ض˿�:%d",udp2.localPort );
			uart1.printf("\r\n��Ϣ��Դ:%d.%d.%d.%d:%d", udp2.remoteIP[0],udp2.remoteIP[1],udp2.remoteIP[2],udp2.remoteIP[3],udp2.remotePort);
			uart1.printf("\r\n��������:");		
			uart1.printf((const char *)udp2.buf);		

		}

	}


}




