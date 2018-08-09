/**
  ******************************************************************************
  * @file    bsp_ebox.cpp
  * @author  cat_li	
  * @version V1.0
  * @date    2018/07/31
  * @brief   Ӳ�������Ϣ����
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

// �����ʹ��object.cpp, ʹ�����з�ʽ����IO����
//mcuGpio 		LED1(GPIOB,GPIO_Pin_8);
//mcuGpio 		LED2(GPIOB,GPIO_Pin_9);
//mcuGpio 		LED3(GPIOB,GPIO_Pin_10);

//mcuGpio			BtnPin(GPIOA,GPIO_Pin_8);
//Uart 				UART(USART1, &PA9, &PA10);


// ʹ��object.cpp,ʹ�����з�ʽ������IO���󣬿��Լ���code������
//#define LED1 PB8
//#define LED2 PB9
//#define LED3 PB10

//#define BtnPin	PA8
//#define	UART		uart1

/*
Led     led2(&PB9,1);
Button  btn(&PA8, 1);
W5500   w5500(&PC13, &PC14, &PC15, &spi2);
SD      sd(&PB12, &spi2);
//Lcd     lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
//W25x16  flash(&PA15, &spi1);
At24c02 eeprom(&i2c2);
*/
CNC cnc;
Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);


/*
*********************************************************************************************************
*	�� �� ��: PrintfLog
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void print_log(const char *name,const char *date)
{
	UART.printf("\n\r");
	UART.printf("*************************************************************\n\r");
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* ��������   : %s\r\n",name);	/* ��ӡ�������� */
	UART.printf("* ��������   : %s\r\n", date);	/* ��ӡ�������� */

	UART.printf("* Ӳ��ƽ̨   : %s \r\n",HARDWARE);
	UART.printf("* EBOX��汾 : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*                     CPU ��Ϣ\r\n");	/* ��ӡһ�пո� */
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* CPUID      : %08X %08X %08X\n\r"
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %dMhz\r\n",cpu.clock.core/1000000);
    UART.printf("* hclk       : %dMhz\r\n",cpu.clock.hclk/1000000);
    UART.printf("* pclk1      : %dMhz\r\n",cpu.clock.pclk1/1000000);
    UART.printf("* pclk2      : %dMhz\r\n",cpu.clock.pclk2/1000000);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}



