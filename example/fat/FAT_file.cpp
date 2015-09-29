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
FIL fsrc;            // �ļ��ṹ

FRESULT res;
u8 ret;
	
SD sd(&PB12,&spi2);

u8 buf[100];
u8 readBuf[6] ;
u32 bw=0;
u32 br=0;

void fileOpt()
{
	for(int i=0;i<100;i++)
		buf[i] = '1';
	res = f_open(&fsrc,"0:12345.txt",FA_WRITE | FA_READ);//û������ļ��򴴽����ļ�
	uart1.printf("\r\n");

	if(res==FR_OK)
	{
		uart1.printf("�����ļ�����ļ��ɹ�  O(��_��)O\r\n");
		uart1.printf("���ļ�����:%d\r\n",fsrc.flag);
		uart1.printf("���ļ���С��%d\r\n",fsrc.fsize);
		uart1.printf("���ļ���д��ʼ����%d\r\n",fsrc.fptr);
//		uart1.printf("���ļ���ʼ�غ�:%d\r\n",fsrc.org_clust);
//		uart1.printf("���ļ���ǰ�غţ�%d\r\n",fsrc.curr_clust);
		uart1.printf("���ļ���ǰ������:%d\r\n",fsrc.dsect);

		f_lseek(&fsrc,0);
		do
		{ 
		 res = f_write(&fsrc, buf, sizeof(buf),&bw); 
		if(res) 
			 { 
				 uart1.printf("write error : %d\r\n",res); 
				 break; 
			 }  
		uart1.printf("write ok!\r\n"); 
		}  
			while (bw < sizeof(buf));  //  �ж��Ƿ�д��(bw > 100����ʾд�����)
	}
	else if(res==FR_EXIST)
		uart1.printf("���ļ��Ѵ���\r\n");
	else
		uart1.printf("�����ļ�����ļ�ʧ��~~~~(>_<)~~~~ %d\r\n",res);
	f_close(&fsrc);//�ر��ļ�
	
	/////////////////////////////////////////////////////////////////////////////////////
	u32 readsize;
	u32 buflen;
	buflen = sizeof(readBuf);
	res = f_open(&fsrc,"0:12345.txt", FA_READ);//û������ļ��򴴽����ļ�
	if(res==FR_OK)
	{
		uart1.printf("���ļ���С��%d\r\n",fsrc.fsize);	
	}
	readsize = 0;
	do
	{
		res=f_read(&fsrc,readBuf,buflen,&br);
		if(res==FR_OK)
		{
//			 uart1.printf("�ɹ���ȡ���ݣ�%dBytes\r\n",br);
//			 uart1.printf("data:%s\r\n",readBuf);
			 uart1.printf_length((const char*)readBuf,sizeof(readBuf));
		}
		else				
			{uart1.printf("��ȡ����ʧ�ܣ�\r\n");}
		readsize+=buflen;
		f_lseek(&fsrc,readsize);
	
	}while(br==buflen);
	uart1.printf("�ļ���ȡ��ĩβ��\r\n");
  f_close(&fsrc);//�ر��ļ�
	f_mount(&fs,"0:",0);


	

}
void setup()
{
	eBoxInit();
	uart1.begin(9600);
			uart1.printf("\r\nsystem start!");

	ret = sd.begin(3);
	if(!ret)
		uart1.printf("\r\nsdcard init ok!");
	attachSDCardToFat(&sd);

	res = f_mount(&fs,"0:",1);
	uart1.printf("res = %d",res);
   
}
u32 count;
int main(void)
{
	setup();
	fileOpt();
	while(1)
	{
		
	//uart1.printf("\r\nrunning��");
		delay_ms(1000);
	}


}




