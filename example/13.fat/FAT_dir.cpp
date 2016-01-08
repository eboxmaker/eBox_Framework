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

extern void attach_sd_to_fat(SD* sd);


static FATFS fs;            // Work area (file system object) for logical drive
FATFS *fss;
DIR DirObject;       //目录结构
FRESULT res;
u8 ret;
	
SD sd(&PB12,&spi2);

void dirOpt()
{
	res=f_mkdir("0:123");//新建目录只能一级一级的建，即调用一次f_mkdir(),建一层目录而已，目录名不能以数字开头
	if(res==FR_OK)
		uart1.printf("\r\n创建目录成功 !");
	else if(res==FR_EXIST)
		uart1.printf("\r\n所建目录已存在 !");    
	else
		uart1.printf("\r\n创建目录失败~~~~(>_<)~~~~");

	res=f_opendir(&DirObject,"0:123");//打开目录
	if(res==FR_OK)
	{
		uart1.printf("\r\n打开目录成功 !");
		uart1.printf("\r\n该目录所在簇号：%d",DirObject.clust);
		uart1.printf("\r\n该目录所在扇区号：%d",DirObject.sect);
	}
	else if(res==FR_NO_PATH)
		uart1.printf("\r\n所找目录路径不存在");    
	else
		uart1.printf("\r\n打开目录失败~~~~(>_<)~~~~");
}
void setup()
{
	ebox_init();
	uart1.begin(9600);
	ret = sd.begin(3);
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	attach_sd_to_fat(&sd);
	
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




