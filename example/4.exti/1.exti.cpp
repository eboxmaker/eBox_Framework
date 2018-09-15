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


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "bsp_ebox.h"

/* ���������������̷������� */
#define EXAMPLE_NAME	"exti example"
#define EXAMPLE_DATE	"2018-08-08"

uint32_t xx;

//����һ���ⲿ�ж϶�������PA8���жϴ���ģʽΪ�½��ش���
Exti ex(&BtnPin, FALLING);

void exit()//�ⲿ�ж��¼�
{
    xx++;
    uart1.printf("\r\nxx = %d", xx);
    LED1.toggle();
}

void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
    LED1.mode(OUTPUT_PP);
    ex.begin();//��ʼ���ⲿ�ж�
    ex.attach(exit);//���ⲿ�жϴ����¼�
    ex.interrupt(ENABLE);//ʹ���ⲿ�ж�

}
int main(void)
{
    setup();
    while(1)
    {
        ;
    }
}
