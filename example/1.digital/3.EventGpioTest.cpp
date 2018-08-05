/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-2	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */
 
 
#include "ebox.h"
#include "EventGpio.h"
#include "EventManager.h"
#include "bsp_ebox.h"

/**
	*	1	此例程需要调用apps目录下的EventGpio , eventio,EventManager模块
	*	2	此例程演示了IO事件响应，分别为低电平，高电平，下降沿，上升沿，单击，释放，长按
	*	3	高定平，低电平会连续触发，触发周期5ms，可通过修改EvenGpio.h中的IO_EDGE_FILTER_COUNTS修改
	* 4	长按时会禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
	* 5 如果没有长按，则下降沿=单击，上升沿=释放
	* 
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"EventGPIO example"
#define EXAMPLE_DATE	"2018-08-02"

static void PrintfLogo(void);
// 下降沿检测
void neg()
{
    UART.println("检测到下降沿信号");
}
// 上升沿检测
void pos()
{
    UART.println("检测到上升沿信号");
}
// 高电平回调函数
void high()
{
    UART.println("检测到高电平");
}

// 高电平回调函数
void low()
{
    UART.println("检测到低电平");
}
// 单击回调函数
void click()
{
    UART.println("检测到按键单击");
}
// 释放回调函数
void release()
{
    UART.println("检测到按键释放");
}
// 长按回调函数
void long_press()
{
    UART.println("检测到长按，长按时不响应单击");
}
void click1()
{
    UART.println("检测到按键单击");
}
void release1()
{
    UART.println("检测到按键释放");
}
void long_press1()
{
    UART.println("检测到长按，长按时不响应单击");
}
//EventGpio event_io_1(&PA8,0,0,0,0,click,release,0);


/** 创建EventGpio对象，并挂载事件回调函数，按顺序分别为高电平，低电平，上升沿，
  *下降沿，单击，释放，长按.不需要响应的事件填写为0
	*/
//EventGpio event_io_2(&PA8,high,low,pos,neg,click1,release1,long_press1);
// 长按时会禁用单击事件，且长按发生后不触发释放事件。但不影响上升沿和下降沿事件
//EventGpio event_io_2(&PA8,0,0,pos,neg,click1,release1,long_press1);
// 如果没有长按，则下降沿=单击，上升沿=释放
EventGpio event_io_2(&PA8,0,0,pos,neg,click1,release1,0);
EventManager io_manager;
void setup()
{
    ebox_init();
    UART.begin(115200);
		PrintfLogo();
    //event_io_1.begin(1);
    event_io_2.begin(1);
    //io_manager.add(&event_io_1);
    io_manager.add(&event_io_2);
}
int main(void)
{
    setup();

    while(1)
    {
        io_manager.process();
        delay_ms(1);
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
