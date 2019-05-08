/**
  ******************************************************************************
  * @file    t_main.cpp
  * @author  ������
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
	*	1	��������ʾ�˼��ı��˵�
	*	2	�˵���������ҳ�棬���˵����ƣ�����led���Ӳ˵���ȡ��������
	*	3	ͨ��UART��ӡ����˵�
	*/


/* ���������������̷������� */
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