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

//Button  btn(&BtnPin,1);
//// ����edriveĿ¼�� ledģ��
//Led     led1(&LED1,0);
//Led     led2(&LED2,0);
//Led     led3(&LED3,0);
//Lcd lcd(&PB5, &PB6, &PB4, &PB3, &spi1);
//W5500Class w5500(&PC13, &PC14, &PC15, &spi2);

//// �����ʹ��object.cpp, ʹ�����з�ʽ����IO����
#if USE_OBJECT == 0
// �����ʹ��object.cpp, ʹ�����з�ʽ����IO����
mcuGpio PA0(PA0_ID);
mcuGpio PA1(PA1_ID);
mcuGpio PA2(PA2_ID);
mcuGpio PA3(PA3_ID);
mcuGpio PA4(PA4_ID);
mcuGpio PA5(PA5_ID);
mcuGpio PA6(PA6_ID);
mcuGpio PA7(PA7_ID);

mcuGpio PA9(PA9_ID);
mcuGpio PA10(PA10_ID);

mcuGpio PA13(PA13_ID);
mcuGpio PA14(PA14_ID);

mcuGpio PB1(PB1_ID);

mcuGpio PF0(PF0_ID);
mcuGpio PF1(PF1_ID);

Uart uart1(USART1, &PA2, &PA3);
#endif


//// ʹ��object.cpp,ʹ�����з�ʽ������IO���󣬿��Լ���code������
////#define   LED1  PB8
////#define   LED2  PB9
////#define   LED3  PB10

////#define   BtnPin	PA8
////#define   UART	uart1

/*
*********************************************************************************************************
*	�� �� ��: PrintfLog
*	����˵��: ��ӡ�������ƺ����̷�������, ���ϴ����ߺ󣬴�PC���ĳ����ն�������Թ۲���
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void print_log(const char *name, const char *date)
{
    UART.print("\n");
    UART.print("*******************************************\n");
    UART.print("* \r\n");	                /* ��ӡһ�пո� */
    UART.print("* ��������      : ");	    /* ��ӡ�������� */
    UART.println(name);
    UART.print("* ��������      : ");	    /* ��ӡ�������� */
    UART.println(date);

    UART.print("* Ӳ��ƽ̨      : ");       /* Ӳ��ƽ̨ */
    UART.println(HARDWARE);
    UART.print("* EBOX��汾    : ebox_V" );/* �汾��Ϣ */
    UART.println(EBOX_VERSION);
    UART.println("******CPU ��Ϣ******");	/* CPU��Ϣ */
    UART.print("* CPU TYPE      : ");	    /* ��ӡCPU���� */
    UART.println(MCU_TYPE);
    UART.print("* CPU PINS      : ");	    /* ��ӡCPU�������� */
    UART.println(STM32_PINS);
    UART.print("* CPU COMPANY   : ");	    /* ��ӡCPU��˾ */
    UART.println(MCU_COMPANY);


    UART.print("* CPUID         : ");       /* ��ӡCPUΨһID */
    for(int i = 0 ; i <11; i++)
    {
        UART.print(cpu.chip_id[i],HEX);
        UART.print("-");
        
    }
    UART.println(cpu.chip_id[11],HEX);



    UART.print("* core          : ");
    UART.print(cpu.clock.core / 1000000.0);	/* ��ӡʱ������Ϣ */
    UART.println("Mhz");
    UART.print("* hclk          : ");
    UART.print(cpu.clock.hclk / 1000000.0);
    UART.println("Mhz");
    UART.print("* pclk1         : ");
    UART.print(cpu.clock.pclk1 / 1000000.0);
    UART.println("Mhz");
    UART.print("* pclk2         : ");
    UART.print(cpu.clock.pclk2 / 1000000.0);
    UART.println("Mhz");

    UART.print("* ability/s     : ");
    UART.print(cpu.ability / 1000000.0);
    UART.println("Mhz");


    UART.print("* flash size    : ");       /* ��ӡflash��С */
    UART.print(cpu.flash.size/1024);
    UART.println("KB");
    
    UART.print("* flash page size    : ");  /* ��ӡflash page��С */
    UART.print(cpu.flash.page_size);
    UART.println("B");


    UART.print("* flash used    : ");	    /* ��ӡflashʹ���˶���KB */
    UART.print(cpu.flash.used / 1024.0);
    UART.println("KB");

    UART.print("* flash remaind : ");	    /* ��ӡflashʣ���˶���KB  */
    UART.print((cpu.flash.size - cpu.flash.used) / 1024.0);
    UART.println("KB");

    UART.print("* mem size      : ");       /* ��ӡSRAM��С */
    UART.print(MCU_SRAM1_SIZE / 1024.0);
    UART.println("KB");

    UART.print("* mem used      : ");	    /* ��ӡSRAMʹ���˶���KB */
    UART.print(MCU_SRAM1_USED / 1024.0);
    UART.println("KB");

    UART.print("* mem remaind   : ");	    /* ��ӡSRAMʣ���˶���KB */
    UART.print(MCU_SRAM1_REMAIND / 1024.0);
    UART.println("KB");

    UART.print("* heap used     : ");	    /* ��ӡheapʹ���� */
    UART.print(ebox_mem_usage());
    UART.println("%");

    UART.print("* heap used     : ");	    /* ��ӡheapʹ���� */
    UART.print(ebox_mem_used() / 1024.0);
    UART.println("KB");

    UART.print("* heap free     : ");	    /* ��ӡheapʹ���˶���KB */
    UART.print(ebox_get_free() / 1024.0);
    UART.println("KB");

    UART.print("*\n");	                /* ��ӡһ�пո� */
    UART.print("*************************************************************\n");
    UART.flush();

}



