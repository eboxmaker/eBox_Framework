/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-2	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "fifo.h"
#include "bsp_ebox.h"

/**
	*	1	��������ʾ�����ʹ��fifo��
	*   ʹ��ebox_fifo_alloc���г�ʼ����ʹ�ö�̬�ڴ�����ռ䡣ע��ʹ����
    *   Ҫ�����ͷ�
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"EventGPIO&EventVar example"
#define EXAMPLE_DATE	"2019-05-17"

Timer timer2(TIM2);
ebox_fifo *fifo;

uint8_t rx[100];
uint8_t source[100];

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    //event_io_1.begin(1);
    
    fifo = ebox_fifo_alloc(128);//�˴�������2��n�η�
    for(int i = 0; i < 100; i++)
        source[i] = i;
    ebox_fifo_put(fifo,source,100);

}
int len;
int main(void)
{
    setup();

    while(1)
    {
        len = ebox_fifo_get(fifo,rx,100);
        if(len > 0)
        {
            UART.print("len:");
            UART.println(len);
            UART.println("data:");
            for(int i = 0; i < len; i++)
                UART.println(rx[i]);
        }
        ebox_fifo_free(fifo);
    }
}
