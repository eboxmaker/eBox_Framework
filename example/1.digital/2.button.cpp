/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-1	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */

#include "ebox.h"
#include "button.h"
#include "bsp_ebox.h"

/**
	*	1	��������Ҫ����eDriveĿ¼�µ�Buttonģ��
	*	2	��������ʾ�˰���  ���£����𣬳��� ���ֲ���
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"Button example"
#define EXAMPLE_DATE	"2018-08-01"

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

// ����btn����IO��������ģʽ
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
        btn.loop();	//��ȡ������ǰ״̬����Ҫ��ʱ����ִ��
			
        if(btn.click())				// �����Ƿ���
        {
            LED1.toggle();
            UART.printf("\r\nclick event!");
        }
        if(btn.release())			// �����ͷ�
        {
            LED2.toggle();
            UART.printf("\r\nrelease event!");
        }
        if(btn.pressed_for(2000, 1)) //�������룬ִ��1��
        {
            LED3.toggle();
            UART.printf("\r\nlong press event!");
        }
				
//				if(btn.pressed_for(2000, 5)) //�������룬ִ��5��
//        {
//            LED3.toggle();
//            UART.printf("\r\nlong press event!");
//        }
    }
}




