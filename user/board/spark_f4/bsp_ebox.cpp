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
#include "ebox_mem.h"
//// �����ʹ��object.cpp, ʹ�����з�ʽ����IO����
////mcuGpio 		LED1(GPIOB,GPIO_Pin_8);
////mcuGpio 		LED2(GPIOB,GPIO_Pin_9);
////mcuGpio 		LED3(GPIOB,GPIO_Pin_10);

////mcuGpio			BtnPin(GPIOA,GPIO_Pin_8);
////Uart 				UART(USART1, &PA9, &PA10);
//mcuSpi spi_w5500(SPI1,&PB3,&PB4,&PA7);
////W5500  w5500(&PA15, &PC14, &PG2, &spi_w5500);
//W5500Class  w5500(&PA15, &PC14, &PG2, &spi_w5500);

//Button  btn(&PH15,1);
//Led     led1(&LED1,0);
//Led     led2(&LED2,0);
//Led     led3(&LED3,0);
//// ʹ��object.cpp,ʹ�����з�ʽ������IO���󣬿��Լ���code������
////#define LED1 PI5
////#define LED2 PI6
////#define LED3 PI7

////#define BtnPin	PA8
////#define	UART		uart1

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
	UART.printf("* ��������      : %s\r\n",name);	/* ��ӡ�������� */
	UART.printf("* ��������      : %s\r\n", date);	/* ��ӡ�������� */

	UART.printf("* Ӳ��ƽ̨      : %s \r\n",HARDWARE);
	UART.printf("* EBOX��汾    : %s (ebox)\r\n", EBOX_VERSION);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*                     CPU ��Ϣ\r\n");	/* ��ӡһ�пո� */
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("* CPU TYPE      : %020d\r\n",MCU_TYPE);	/* ��ӡһ�пո� */
	UART.printf("* CPU PINS      : %-020d\r\n",STM32_PINS);	/* ��ӡһ�пո� */
	UART.printf("* CPU COMPANY   : %s\r\n",MCU_COMPANY);	/* ��ӡһ�пո� */
	UART.printf("* CPUID         : %-010.2f %08X %08X\n\r"
			, -123.456, cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* core          : %0.3fMhz\r\n",cpu.clock.core/1000000.0);
    UART.printf("* hclk          : %0.3fMhz\r\n",cpu.clock.hclk/1000000.0);
    UART.printf("* pclk1         : %0.3fMhz\r\n",cpu.clock.pclk1/1000000.0);
    UART.printf("* pclk2         : %0.3fMhz\r\n",cpu.clock.pclk2/1000000.0);
    
	UART.printf("* flash size    : %d    \tKB \r\n",cpu.flash_size);
	UART.printf("* flash used    : %0.3f \tKB\r\n",MCU_FLASH_USED/1024.0);	/* ��ӡһ�пո� */   
	UART.printf("* flash remaind : %0.3f \tKB\r\n",MCU_FLASH_REMAIND/1024.0);	/* ��ӡһ�пո� */ 
    
	UART.printf("* mem size      : %0.3f \tKB \r\n",MCU_SRAM1_SIZE/1024.0);
	UART.printf("* mem used      : %0.3f \tKB\r\n",MCU_SRAM1_USED/1024.0);	/* ��ӡһ�пո� */   
	UART.printf("* mem remaind   : %0.3f \tKB\r\n",MCU_SRAM1_REMAIND/1024.0);	/* ��ӡһ�пո� */   
	UART.printf("* heap used     : %0.3f \tKB\r\n",ebox_mem_used()/1024.0);	/* ��ӡһ�пո� */   
	UART.printf("* heap free     : %0.3f \tKB\r\n",ebox_get_free()/1024.0);	/* ��ӡһ�пո� */   
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}




