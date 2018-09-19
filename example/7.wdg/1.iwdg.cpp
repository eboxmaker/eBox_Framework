/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


/*
һ���������Ź���ʾ������
*/
#include "ebox.h"
#include "bsp_ebox.h"
/* ���������������̷������� */
#define EXAMPLE_NAME	"independent wdg example"
#define EXAMPLE_DATE	"2018-08-08"

Iwdg dog;


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    UART.printf("reset !!!\r\n");
    dog.begin(26208);
}

int main(void)
{

    setup();

    while(1)
    {
        dog.feed();//�ڳ�����Ӧ����ι��Ӧ���ڶ�ʱ���ж��ж�ʱι��
        UART.printf("running!\r\n");
        delay_ms(25000);
        //        delay_ms(1900);
        //        delay_ms(2000);
        //        delay_ms(3000);
    }
}




