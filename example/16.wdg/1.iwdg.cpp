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

Iwdg dog;


void setup()
{
    ebox_init();
    uart1.begin(9600);
    uart1.printf("reset !!!\r\n");
    dog.begin(2000);
}

int main(void)
{

    setup();

    while(1)
    {
        dog.feed();//�ڳ�����Ӧ����ι��Ӧ���ڶ�ʱ���ж��ж�ʱι��
        uart1.printf("running!\r\n");
        delay_ms(1000);
        //        delay_ms(1900);
        //        delay_ms(2000);
        //        delay_ms(3000);
    }
}




