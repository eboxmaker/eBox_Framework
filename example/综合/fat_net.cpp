
#include "ebox.h"
#include "w5500.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "udp.h"
extern void attachSDCardToFat(SD* sd);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //Ŀ¼�ṹ
DWORD free_clust;//�մأ���������С

u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*����Mac����*/
u8 lip[4]={192,168,1,111};/*����lp����*/
u8 sub[4]={255,255,255,0};/*����subnet����*/
u8 gw[4]={192,168,1,1};/*����gateway����*/

u8 ip[6];
	
W5500 w5500(PC13,SPI2,PB13,PB14,PB15,PC14,PC15);
SD sd(PB12,SPI2,PB13,PB14,PB15);

UDP udp1;

uint32_t ret;

u8 buf[1024];
u32 rl;
float x;
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	ret = sd.begin();
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	f_mount(0, &fs);

	w5500.begin(mac,lip,sub,gw);

	attachEthToSocket(&w5500);
	attachSDCardToFat(&sd);

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
	uart1.printf("\r\n���� = %f",x/1024/1024);	
   
  res=f_getfree("/",&free_clust,&fss);
       uart1.printf("�÷�������������Ϊ��%d\r\n",(fss->max_clust-2)*(fss->csize));
	if(res==FR_OK)
	{
		uart1.printf("�÷�������������Ϊ��%d\r\n",(fss->max_clust-2)*(fss->csize));
		uart1.printf("�÷�����СΪ��%dM\r\n",(fss->max_clust-2)*(fss->csize)/2048);
		uart1.printf("�÷����մ���Ϊ��%d\r\n",free_clust);
		uart1.printf("�÷�����������Ϊ��%d\r\n",free_clust*(fss->csize));
	}
	else
		uart1.printf("��ȡ�����մ�ʧ��\r\n,res = %d",res);
	uart1.printf("\r\n");

}
u32 count;
int main(void)
{
	setup();
	while(1)
	{
	  res=f_opendir(&DirObject,"/123");//��Ŀ¼
    if(res==FR_OK)
    {
      uart1.printf("\r\n��Ŀ¼�ɹ� !\r\n");
      uart1.printf("��Ŀ¼���ڴغţ�%d\r\n",DirObject.clust);
      uart1.printf("��Ŀ¼���������ţ�%d\r\n",DirObject.sect);
    }
    else if(res==FR_NO_PATH)
      uart1.printf("����Ŀ¼·��������\r\n");    
    else
      uart1.printf("��Ŀ¼ʧ��~~~~(>_<)~~~~ \r\n");
		if(udp1.recv(buf))
		{
			uart1.printf("\r\n============================");		
			uart1.printf("\r\n���ض˿�:%d",udp1.localPort );
			uart1.printf("\r\n��Ϣ��Դ:%d.%d.%d.%d:%d", udp1.remoteIP[0],udp1.remoteIP[1],udp1.remoteIP[2],udp1.remoteIP[3],udp1.remotePort);
			uart1.printf("\r\n��������:");		
			uart1.printf((const char *)buf);		
			uart1.printf("\r\n============================");		
			udp1.sendto(udp1.remoteIP,udp1.remotePort,buf,100);
		}
		delay_ms(1000);
	}


}




