/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *					 2018-8-5	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������ 
	* Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */



#include "ebox.h"
#include "math.h"
#include "bsp_ebox.h"

/**
	*	1	��������ʾ��ģ��DAC����
	*	2	ʵ��������PWM���Ƶ��1Khz��ռ�ձȿɵ��ķ�����0-1000��
	*	3	ʵ������ʱLED����
	* 4	������Դ��ο�ebox_pwm.h˵��
	*/
	

/* ���������������̷������� */
#define EXAMPLE_NAME	"Analog_Write example"
#define EXAMPLE_DATE	"2018-08-05"


static void PrintfLogo(void);

void setup()
{
    ebox_init();
		UART.begin(115200);
		PrintfLogo();		
}

float x;
uint16_t y;
int main(void)
{

    setup();

    while(1)
    {
        x = x + PI * 0.01;
        if(x >= PI)x = 0;
        y = 2000 - (sin(x) + 1) * 1000;

        analog_write(&DAC_0, y);
        delay_ms(20);
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
