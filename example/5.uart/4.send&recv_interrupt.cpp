 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  *		 2018-8-6	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
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

/**
    *	1	��������ʾ���ڵķ���&�����ж�
	*   2	�յ����ݺ��ͬʱ�����ݷ��أ������ڳ����ն���Ļ���Ч��
	*	3	�������ʵ��Ч�������л����͸�ʽ�����鲻Ҫʹ��hex��ʽ
	*/
#include "ebox.h"
#include "bsp_ebox.h"


/* ���������������̷������� */
#define EXAMPLE_NAME	"send&recv_interrupt example"
#define EXAMPLE_DATE	"2018-08-06"


static void PrintfLogo(void);

u8 count;
// �����жϻص�
void rx_event()
{
    uint8_t c;
    c = UART.read();
    UART.write(c);
	LED1.toggle();
}
// �����жϻص�����תLED״̬
void tc_evnet()
{
    count++;
    LED2.toggle();
}
void setup()
{
    ebox_init();
    UART.begin(115200);
    PrintfLogo();
    // ����Ӧ�жϣ���ʹ��
    UART.attach(rx_event,RxIrq);
    UART.attach(tc_evnet,TcIrq);
    UART.interrupt(RxIrq,ENABLE);
    UART.interrupt(TcIrq,ENABLE);
    
    LED1.mode(OUTPUT_PP);
    LED1.reset();
    LED2.mode(OUTPUT_PP);
    LED2.reset();
}

int main(void)
{
    setup();
    while(1)
    {
        //uart1.printf("uart is ok ! count = %d\r\n", count);
        delay_ms(1000);
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
			, cpu.chip_id[2], cpu.chip_id[1], cpu.chip_id[0]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
    UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
    UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}


