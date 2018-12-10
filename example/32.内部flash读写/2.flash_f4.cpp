/*
file   : *.cpp
author : shentq
version: V2.2
date   : 2018/8/8

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/**
    *	1	一个F4系列单片机的flash读写测试程序
	*   2	地址必须4字节对齐
	*/

#include "ebox.h"
#include "bsp_ebox.h"
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"STM32F4xx flash read and write example"
#define EXAMPLE_DATE	"2018-08-08"

uint32_t buf1[10] = {0x12345678, 1, 2, 3, 4, 5, 6, 7, 8, 9};
uint32_t buf2[10] = {};
static void PrintfLogo(void);

Flash flash;
void setup()
{
    ebox_init();
    UART.begin(115200);
    PrintfLogo();

    flash.write(ADDR_FLASH_SECTOR_4, buf1, 10);
    flash.read(ADDR_FLASH_SECTOR_4, buf2, 10);
    for(int i = 0 ; i < 10; i++)
    {
        UART.printf("0x%x\r\n", buf2[i]);
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
*	函 数 名: PrintfLogo
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void PrintfLogo(void)
{
    UART.printf("\n\r");
    UART.printf("*************************************************************\n\r");
    UART.printf("* \r\n");	/* 打印一行空格 */
    UART.printf("* 例程名称   : %s\r\n", EXAMPLE_NAME);	/* 打印例程名称 */
    UART.printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

    UART.printf("* 硬件平台   : %s \r\n", HARDWARE);
    UART.printf("* EBOX库版本 : %s (ebox)\r\n", EBOX_VERSION);
    UART.printf("* \r\n");	/* 打印一行空格 */
    UART.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
    UART.printf("* \r\n");	/* 打印一行空格 */
    UART.printf("* CPUID      : %08X %08X %08X\n\r"
                , cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
    UART.printf("* flash size : %d KB \r\n", cpu.flash_size);
    UART.printf("* core       : %d\r\n", cpu.clock.core);
    UART.printf("* hclk       : %d\r\n", cpu.clock.hclk);
    UART.printf("* pclk1      : %d\r\n", cpu.clock.pclk1);
    UART.printf("* \r\n");	/* 打印一行空格 */
    UART.printf("*************************************************************\n\r");
}






