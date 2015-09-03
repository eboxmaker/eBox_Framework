
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
DIR DirObject;       //Ŀ¼�ṹ
FRESULT res;
u8 ret;
	
SD sd(PB12,SPI2,PB13,PB14,PB15);

void dirOpt()
{
	res=f_mkdir("/123");//�½�Ŀ¼ֻ��һ��һ���Ľ���������һ��f_mkdir(),��һ��Ŀ¼���ѣ�Ŀ¼�����������ֿ�ͷ
	if(res==FR_OK)
		uart1.printf("\r\n����Ŀ¼�ɹ� !");
	else if(res==FR_EXIST)
		uart1.printf("\r\n����Ŀ¼�Ѵ��� !");    
	else
		uart1.printf("\r\n����Ŀ¼ʧ��~~~~(>_<)~~~~");

	res=f_opendir(&DirObject,"/123");//��Ŀ¼
	if(res==FR_OK)
	{
		uart1.printf("\r\n��Ŀ¼�ɹ� !");
		uart1.printf("\r\n��Ŀ¼���ڴغţ�%d",DirObject.clust);
		uart1.printf("\r\n��Ŀ¼���������ţ�%d",DirObject.sect);
	}
	else if(res==FR_NO_PATH)
		uart1.printf("\r\n����Ŀ¼·��������");    
	else
		uart1.printf("\r\n��Ŀ¼ʧ��~~~~(>_<)~~~~");
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
	dirOpt();
	while(1)
	{
		
		delay_ms(1000);
	}


}




