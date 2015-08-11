
#include "ebox.h"
#include "w5500.h"
USART uart1(USART1,PA9,PA10);


  u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*����Mac����*/
  u8 lip[4]={192,168,1,111};/*����lp����*/
  u8 sub[4]={255,255,255,0};/*����subnet����*/
  u8 gw[4]={192,168,1,1};/*����gateway����*/
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

	
	 eth.setSHAR(mac);/*����Mac��ַ*/
   eth.setSUBR(sub);/*������������*/
   eth.setGAR(gw);/*����Ĭ������*/
   eth.setSIPR(lip);/*����Ip��ַ*/
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




