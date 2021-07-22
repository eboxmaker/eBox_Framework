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
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    UART.printf("reset !!!\r\n");
    dog.begin(4000);
}

int main(void)
{
    uint32_t current, i = 0;
    setup();
    current = millis();
    while(1)
    {
        dog.feed();//�ڳ�����Ӧ����ι��Ӧ���ڶ�ʱ���ж��ж�ʱι��
        UART.printf("feed %d running %.02fs interval %.02f s!\r\n", i++, (millis() / 1000.0), (float)((millis() - current) / 1000.0));
        current = millis();
        delay_ms(5000);
    }
}




