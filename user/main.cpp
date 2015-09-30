
#include "ebox.h"
#include "w5500.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "udp.h"
extern void attach_sd_to_fat(SD* sd);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //目录结构
DWORD free_clust;//空簇，空扇区大小

u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
u8 lip[4]={192,168,1,111};/*定义lp变量*/
u8 sub[4]={255,255,255,0};/*定义subnet变量*/
u8 gw[4]={192,168,1,1};/*定义gateway变量*/

u8 ip[6];
	
W5500 w5500(&PC13,&PC14,&PC15,&spi2);
SD sd(&PB12,&spi2);

UDP udp1;

uint32_t ret;

u8 buf[1024];
u32 rl;
float x;
void setup()
{
	ebox_init();
	uart1.begin(9600);
	ret = sd.begin(1);
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");

	w5500.begin(2,mac,lip,sub,gw);

	attach_eth_to_socket(&w5500);
	attach_sd_to_fat(&sd);

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


	uart1.printf("\r\nret = %d",ret);
	ret = sd.getCID(buf);
	uart1.printf("\r\nret = %d",ret);
	uart1.printf((const char*)buf);

	ret = sd.getCSD(buf);
	uart1.printf("\r\nret = %d",ret);

	rl = sd.getCapacity();
	x = (float)rl;
	uart1.printf("\r\n容量 = %f",x/1024/1024);	
   

	uart1.printf("\r\n");

}
u32 count;
int main(void)
{
	setup();
	while(1)
	{

		if(udp1.recv(buf))
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n本地端口:%d",udp1.localPort );
			uart1.printf("\r\n消息来源:%d.%d.%d.%d:%d", udp1.remoteIP[0],udp1.remoteIP[1],udp1.remoteIP[2],udp1.remoteIP[3],udp1.remotePort);
			uart1.printf("\r\n数据内容:");		
			uart1.printf((const char *)buf);		
			uart1.printf("\r\n============================");		
			udp1.sendto(udp1.remoteIP,udp1.remotePort,buf,100);
		}
		delay_ms(1000);
	}


}




