/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "mmc_sd.h"
#include "ff.h"

extern void attachSDCardToFat(SD* sd);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
DIR DirObject;       //Ŀ¼�ṹ
FRESULT res;
u8 ret;
	
SD sd(&PB12,&spi2);

void dirOpt()
{
	res=f_mkdir("0:123");//�½�Ŀ¼ֻ��һ��һ���Ľ���������һ��f_mkdir(),��һ��Ŀ¼���ѣ�Ŀ¼�����������ֿ�ͷ
	if(res==FR_OK)
		uart1.printf("\r\n����Ŀ¼�ɹ� !");
	else if(res==FR_EXIST)
		uart1.printf("\r\n����Ŀ¼�Ѵ��� !");    
	else
		uart1.printf("\r\n����Ŀ¼ʧ��~~~~(>_<)~~~~");

	res=f_opendir(&DirObject,"0:123");//��Ŀ¼
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
	ret = sd.begin(3);
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	attachSDCardToFat(&sd);
	
	res = f_mount(&fs,"0:",1);
	uart1.printf("\r\nres = %d",res);
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




