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
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    
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
