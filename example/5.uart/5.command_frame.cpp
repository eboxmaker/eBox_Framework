 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
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

/**
    *	1	һ���򵥵�����֡����ʾ�����ʹ������֡��ͨѶ
	*   2	$֡ͷ�����߳���Ҫ��ʼ����������
	*	3	��֡β�����߳������ݷ������
    *   4   ���������ɺ󣬴�ӡ������ܵ������ݣ���׼���´ν���
	*/
#include "ebox.h"
#include "bsp_ebox.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"command frame example"
#define EXAMPLE_DATE	"2018-08-06"


static void PrintfLogo(void);


#define  HEAD '$'
#define  END '!'

#define  NEEDHEAD 0
#define  NEEDDATA 1
#define  DATAEND 2

uint8_t state = NEEDHEAD;

char rcv[100];                      // ������ܵ������ݣ����100
int i;
void test()
{
    LED1.toggle();
    uint8_t c;
    c = UART.read();
    switch(state)
    {
    case NEEDHEAD:
        if(c == HEAD)
        {
            i = 0;
            rcv[i++] = c;
            state = NEEDDATA;
            //UART.printf("���������ݣ�����������ɺ��� ����β\r\n");   // ����˾�ص�֮������ԭ����
        }
        break;
    case NEEDDATA:
        if(c == END)
        {
            rcv[i] = c;
            state = DATAEND;
        }
        else
        {
            rcv[i++] = c;
        }
        break;
    }


}

void setup()
{
    ebox_init();
    UART.begin(115200);
    PrintfLogo();
    UART.attach(test,RxIrq);
    UART.interrupt(RxIrq,ENABLE);
}

float x, y;
int main(void)
{

    setup();
    UART.printf("����������֡ͷ $ \r\n");
    while(1)
    {
        if(state == DATAEND)
        {

            UART.printf(rcv);
            for(int i = 0; i < 100; i ++)
                rcv[i] = 0;
            state = NEEDHEAD;
        }

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


