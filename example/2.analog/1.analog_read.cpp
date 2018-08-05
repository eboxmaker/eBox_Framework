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
#include "bsp_ebox.h"

/**
	*	1	��������ʾ��ADC��ȡԭʼhexֵ�͵�ѹֵ
	*	2	ebox ADCͨ��DMA������Ĭ��Ϊ����״̬
	*	3	ϵͳ72Mʱ��Ƶ����ADCʱ��12M����������Ϊ54.4us
	* 4	analog_read ����������ȡ������ֵ�������Ϊ54.4us
	* 5 ����Ϊ12λ������analog_read����ֵ��ΧΪ0-4095
	* 6	֧��ADC1��16��ͨ�����ݲ�֧���ڲ�ͨ��
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"Analog_Read example"
#define EXAMPLE_DATE	"2018-08-05"

int16_t x;
static void PrintfLogo(void);
void setup()
{
    ebox_init();
    UART.begin(115200);
		PrintfLogo();
    ADC_0.mode(AIN);
		ADC_1.mode(AIN);
}

int main(void)
{
    setup();
    while(1)
    {
        x = analog_read(&ADC_0);
        UART.printf("ADC0 hex = %05d\r\n", x);
        x = analog_read_voltage(&ADC_0);
        UART.printf("ADC0 val = %04dmv\r\n", x);
			
			  x = analog_read(&ADC_1);
        UART.printf("ADC1 hex = %05d\r\n", x);
        x = analog_read_voltage(&ADC_1);
        UART.printf("ADC1 val = %04dmv\r\n", x);
        UART.printf("==============\r\n", x);
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
			, cpu.chip_id[2], cpu.chip_id[1]);
	UART.printf("* flash size : %d KB \r\n",cpu.flash_size);
	UART.printf("* core       : %d\r\n",cpu.clock.core);
  UART.printf("* hclk       : %d\r\n",cpu.clock.hclk);
  UART.printf("* pclk1      : %d\r\n",cpu.clock.pclk1);
	UART.printf("* \r\n");	/* ��ӡһ�пո� */
	UART.printf("*************************************************************\n\r");
}

