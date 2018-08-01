/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-1	通过引入bsp，定义硬件端口，方便例程可以在不同平台上运行
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */

#include "ebox.h"
#include "button.h"
#include "bsp_ebox.h"

/**
	*	1	此例程需要调用eDrive目录下的Button模块
	*	2	此例程演示了按键  按下，弹起，长按 三种操作
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"Button example"
#define EXAMPLE_DATE	"2018-08-01"

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

// 创建btn对象，IO上拉输入模式
Button btn(&BtnPin,1);

void setup()
{
    ebox_init();
    UART.begin(115200);
		PrintfLogo();
    btn.begin();
    LED1.mode(OUTPUT_PP);
    LED2.mode(OUTPUT_PP);
    LED3.mode(OUTPUT_PP);
}



int main(void)
{
    setup();
    while(1)
    {
        btn.loop();	//获取按键当前状态，需要定时调用执行
			
        if(btn.click())				// 按键是否按下
        {
            LED1.toggle();
            UART.printf("\r\nclick event!");
        }
        if(btn.release())			// 按键释放
        {
            LED2.toggle();
            UART.printf("\r\nrelease event!");
        }
        if(btn.pressed_for(2000, 1)) //长按两秒，执行1次
        {
            LED3.toggle();
            UART.printf("\r\nlong press event!");
        }
				
//				if(btn.pressed_for(2000, 5)) //长按两秒，执行5次
//        {
//            LED3.toggle();
//            UART.printf("\r\nlong press event!");
//        }
    }
}




