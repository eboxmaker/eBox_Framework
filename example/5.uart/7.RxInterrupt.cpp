 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
	*					 2018-8-5	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


#include "ebox.h"
#include "bsp_ebox.h"

/**
	*	1	此例程演示串口的接收中断
	* 2	收到数据后会同时将数据返回，类似于超级终端里的回显效果
	*	2	如果不能实现效果，请切换发送格式，建议不要使用hex格式
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"RxInterrupt example"
#define EXAMPLE_DATE	"2018-08-05"

static void PrintfLogo(void);

// 串口接收中断回调函数，收到数据后返回，并翻转LED
void rx_event()
{
    uint8_t c;
    c = uart1.read();
    uart1.write(c);
    LED1.toggle();
}
void setup()
{
    ebox_init();
    UART.begin(115200);
		PrintfLogo();
    UART.attach(rx_event,RxIrq);
    UART.interrupt(RxIrq,ENABLE);
    LED1.mode(OUTPUT_PP);
    LED1.reset();
		UART.printf("please enter a word to mcu\r\n");
}

int main(void)
{
    setup();
    while(1)
    {        
        //delay_ms(1000);
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
	UART.printf("* 例程名称   : %s\r\n",EXAMPLE_NAME);	/* 打印例程名称 */
	UART.printf("* 发布日期   : %s\r\n", EXAMPLE_DATE);	/* 打印例程日期 */

	UART.printf("* 硬件平台   : %s \r\n",HARDWARE);
	UART.printf("* EBOX库版本 : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
  UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
  UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*************************************************************\n\r");
}



