/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li	
  * @version V1.0
  * @date    2018/07/31
  * @brief   硬件相关信息声明
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


/* Includes ------------------------------------------------------------------*/
#include "bsp_ebox.h"

// 如果不使用object.cpp, 使用下列方式创建IO对象
//mcuGpio 		LED1(GPIOB,GPIO_Pin_8);
//mcuGpio 		LED2(GPIOB,GPIO_Pin_9);
//mcuGpio 		LED3(GPIOB,GPIO_Pin_10);

//mcuGpio			BtnPin(GPIOA,GPIO_Pin_8);
//Uart 				UART(USART1, &PA9, &PA10);
mcuSpi spi_w5500(SPI1,&PB3,&PB4,&PA7);
W5500  w5500(&PA15, &PC14, &PG2, &spi_w5500);
Button btn(&PH15,1);
Led     led1(&LED1,0);
Led     led2(&LED2,0);
Led     led3(&LED3,0);
// 使用object.cpp,使用下列方式重命名IO对象，可以减少code区消耗
//#define LED1 PI5
//#define LED2 PI6
//#define LED3 PI7

//#define BtnPin	PA8
//#define	UART		uart1

/*
*********************************************************************************************************
*	函 数 名: PrintfLog
*	功能说明: 打印例程名称和例程发布日期, 接上串口线后，打开PC机的超级终端软件可以观察结果
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void print_log(const char *name,const char *date)
{
	UART.printf("\n\r");
	UART.printf("*************************************************************\n\r");
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("* 例程名称   : %s\r\n",name);	/* 打印例程名称 */
	UART.printf("* 发布日期   : %s\r\n", date);	/* 打印例程日期 */

	UART.printf("* 硬件平台   : %s \r\n",HARDWARE);
	UART.printf("* EBOX库版本 : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*                     CPU 信息\r\n");	/* 打印一行空格 */
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
    UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
    UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
    UART.printf("* pclk2      : %d\r\n",cpu.clock.pclk2);
	UART.printf("* \r\n");	/* 打印一行空格 */
	UART.printf("*************************************************************\n\r");
}




