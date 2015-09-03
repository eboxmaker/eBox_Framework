
#include "ebox.h"
#include "w5500.h"
#include "mmc_sd.h"
#include "ff.h"
#include "wrapperdiskio.h"
#include "udp.h"
extern void attachSDCardToFat(SD* sd);

USART uart1(USART1,PA9,PA10);

static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //Ŀ¼�ṹ
DWORD free_clust;//�մأ���������С

	
SD sd(PB12,SPI2,PB13,PB14,PB15);

UDP udp1;

u8 ret;

u8 buf[1024];
u32 rl;
float x;
void getSDCardInfo()
{
	ret = sd.getCID(buf);
	uart1.printf("\r\n========================");
	uart1.printf("\r\nget CID Info,ret = %d",ret);
	uart1.printf("\r\n");
	uart1.printf((const char*)buf);

	rl = sd.getCapacity();
	uart1.printf("\r\n========================");
	uart1.printf("\r\n���� = %d",rl/1024/1024);	

	
	res=f_getfree("/",&free_clust,&fss);
	uart1.printf("\r\n�÷�������������Ϊ��%d",(fss->max_clust-2)*(fss->csize));
	if(res==FR_OK)
	{
		uart1.printf("\r\n�÷�������������Ϊ��%d",(fss->max_clust-2)*(fss->csize));
		uart1.printf("\r\n�÷�����СΪ��%dM",(fss->max_clust-2)*(fss->csize)/2048);
		uart1.printf("\r\n�÷����մ���Ϊ��%d",free_clust);
		uart1.printf("\r\n�÷�����������Ϊ��%d",free_clust*(fss->csize));
	}
	else
		uart1.printf("\r\n��ȡ�����մ�ʧ��,res = %d",res);
	uart1.printf("\r\n OVER !");

	
}
void setup()
{
	eBoxInit();
	uart1.begin(9600);
	ret = sd.begin();
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	attachSDCardToFat(&sd);
	
	f_mount(0, &fs);
   
}
u32 count;
int main(void)
{
	setup();
	getSDCardInfo();
	while(1)
	{
		
		delay_ms(1000);
	}


}




