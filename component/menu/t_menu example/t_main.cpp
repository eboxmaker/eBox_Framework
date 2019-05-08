/**
  ******************************************************************************
  * @file    t_main.cpp
  * @author  好心情
  * @version V2.0
  * @date    2019/03/18
  * @brief   ebox application example .
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


#include "ebox.h"
#include "bsp_ebox.h"
#include "..\component\menu\t_menu example\t_menu.h"

/**
	*	1	此例程演示了简单文本菜单
	*	2	菜单包含两级页面，父菜单控制，开关led，子菜单获取串口输入
	*	3	通过UART打印输出菜单
	*/


/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"menu example"
#define EXAMPLE_DATE	"2018-08-08"

void setup()
{
    ebox_init();
    UART.begin(115200);
		print_log(EXAMPLE_NAME,EXAMPLE_DATE);
		UART.setTimeout(200);
		LED1.mode(OUTPUT_PP);
		UART.printf("\r\n");
		displayCurrentMenu(&s_tMenuDemo);
}

int main(void)
{
    setup();
		while(1){
			menu_task(&s_tMenuDemo);
		}
}