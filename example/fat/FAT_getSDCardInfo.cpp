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

USART uart1(USART1,&PA9,&PA10);

static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
FRESULT res;
DIR DirObject;       //目录结构
DWORD free_clust;//空簇，空扇区大小

	
SD sd(&PB12,&spi2);


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
	uart1.printf("\r\n容量 = %d",rl/1024/1024);	

	
	res=f_getfree("/",&free_clust,&fss);
	if(res==FR_OK)
	{
		uart1.printf("\r\n该分区空闲扇区数为：%d",(fss->free_clust)*(fss->csize));
		uart1.printf("\r\n该分区大小为：%dM",(fss->free_clust)*(fss->csize)/2048);
		uart1.printf("\r\n该分区空簇数为：%d",free_clust);
		uart1.printf("\r\n该分区空扇区数为：%d",free_clust*(fss->csize));
	}
	else
		uart1.printf("\r\n获取分区空簇失败,res = %d",res);
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
	
	res = f_mount(&fs,"0:",1);
	uart1.printf("\r\nres = %d",res);
   
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




