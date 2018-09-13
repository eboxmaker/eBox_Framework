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

uint8_t count;
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
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
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



