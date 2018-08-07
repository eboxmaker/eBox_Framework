/*
file   : *.cpp
author : shentq
version: V2.2
date   : 2018/8/8

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/**
    *	1	һ��F4ϵ�е�Ƭ����flash��д���Գ���
	*   2	��ַ����4�ֽڶ���
	*/
    
#include "ebox.h"
#include "bsp_ebox.h"
/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F4xx flash read and write example"
#define EXAMPLE_DATE	"2018-08-08"

uint32_t buf1[10]={0x12345678,1,2,3,4,5,6,7,8,9};
uint32_t buf2[10]={};
static void PrintfLogo(void);

Flash flash;
void setup()
{
    ebox_init();
    UART.begin(115200);
    PrintfLogo();

    flash.write(ADDR_FLASH_SECTOR_4,buf1,10);
    flash.read(ADDR_FLASH_SECTOR_4,buf2,10);
    for(int i = 0 ; i < 10; i++)
    {
        UART.printf("0x%x\r\n",buf2[i]);
    }
}

int main(void)
{

    setup();

    while(1)
    {
    }
}
/*
*********************************************************************************************************
*	�� �� ��: PrintfLogo
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
	UART.printf("\n\r");
	UART.printf("*************************************************************\n\r");
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* ��������   : %s\r\n",EXAMPLE_NAME);	/* ��ӡ�������� */
	UART.printf("* ��������   : %s\r\n", EXAMPLE_DATE);	/* ��ӡ�������� */

	UART.printf("* Ӳ��ƽ̨   : %s \r\n",HARDWARE);
	UART.printf("* EBOX��汾 : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*                     CPU ��Ϣ\r\n");	/* ��ӡһ�пո� */
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
    UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
    UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}






