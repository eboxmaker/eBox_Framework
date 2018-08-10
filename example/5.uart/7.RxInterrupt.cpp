 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/08/14
  * @brief   ebox application example .
  *		 2018-8-5	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
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
	*   2	�յ����ݺ��ͬʱ�����ݷ��أ������ڳ����ն���Ļ���Ч��
	*	3	�������ʵ��Ч�������л����͸�ʽ�����鲻Ҫʹ��hex��ʽ
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"RxInterrupt example"
#define EXAMPLE_DATE	"2018-08-05"


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
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

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

