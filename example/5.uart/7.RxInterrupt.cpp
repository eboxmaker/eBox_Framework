 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
	*					 2018-8-5	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
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
	*	1	��������ʾ���ڵĽ����ж�
	* 2	�յ����ݺ��ͬʱ�����ݷ��أ������ڳ����ն���Ļ���Ч��
	*	2	�������ʵ��Ч�������л����͸�ʽ�����鲻Ҫʹ��hex��ʽ
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"RxInterrupt example"
#define EXAMPLE_DATE	"2018-08-05"

static void PrintfLogo(void);

// ���ڽ����жϻص��������յ����ݺ󷵻أ�����תLED
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
			, cpu.chip_id[2], cpu.chip_id[1]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
  UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
  UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}



