
#include "ebox.h"
#include "w5500.h"
USART uart1(USART1,PA9,PA10);


  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
  u8 lip[4]={192,168,1,111};/*定义lp变量*/
  u8 sub[4]={255,255,255,0};/*定义subnet变量*/
  u8 gw[4]={192,168,1,1};/*定义gateway变量*/
  u8 ip[6];
	
W5500 eth(PA4,SPI1,PA5,PA6,PA7,PA2,PA3);
void setup()
{
	eBoxInit();
	uart1.begin(115200);
	uart1.printf("afd");
	uart1.printf("afd");
	
	eth.begin();
	eth.reset();

	
	 eth.setSHAR(mac);/*配置Mac地址*/
   eth.setSUBR(sub);/*配置子网掩码*/
   eth.setGAR(gw);/*配置默认网关*/
   eth.setSIPR(lip);/*配置Ip地址*/
//	uart1.printf("afd");

}

int16_t x;
int main(void)
{
	setup();

  eth.getSHAR (ip);
  uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", ip[0],ip[1],ip[2],ip[3],ip[4],ip[5]);
  eth.getSIPR (ip);
  uart1.printf("IP : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  eth.getSUBR(ip);
  uart1.printf("mask : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  eth.getGAR(ip);
  uart1.printf("GW : %d.%d.%d.%d\r\n", ip[0],ip[1],ip[2],ip[3]);
  uart1.printf("Network is ready.\r\n");
	while(1)
	{
//		PA1->write(1);
		delay_ms(1000);

	}


}




