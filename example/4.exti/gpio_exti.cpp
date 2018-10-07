 /**
  ******************************************************************************
  * @file    main.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2017/07/13
  * @brief   ebox exti example, ����stm32nucleo(072)ƽ̨��֤	
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

/**
	*	1	��������ʾ��GPIO�ж�
    *   2   ����userbt1�����û����������º͵���󶨲�ͬ�Ļص�����
            ���𴮿ڴ�ӡ��Ϣ�����·�תled���
    *   3   ex����PA0���½��غ������ذ�ͬһ���ص�������x++���Ӵ������
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"STM32F0 GPIO_EXTI example"
#define EXAMPLE_DATE	"2017-09-10"


uint32_t xx;


Exti   userbt1(&BtnPin);
Exti   ex(&PA0);

/**
 *@brief    ��̬�ص�����
 *@param    none
 *@retval   none
*/
void fall()
{
    xx--;
    UART.printf("\r\n falling,xx = %d", xx);
}

void rise()
{
    xx++;
    UART.printf("\r\n rising, xx = %d", xx);
}

void fallrise(){
		xx++;
		UART.printf("\r\n fallrise, xx = %d", xx);
}

/**
 *@brief    �����࣬�����������Ա��
 *@param    none
 *@retval   none
*/
class Test 
{
    public:
    void event() 
    {
        LED1.toggle();
    }
};
Test test;

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);
	
    LED1.mode(OUTPUT_PP);
	// �����أ��½��ؾ�����,��ͬһ���жϻص�����
    ex.begin();
    ex.attach(fallrise,FALL_RISING);
    ex.enable(FALL_RISING);

    // �����أ��½��ص��ò�ͬ�Ļص�����
    userbt1.begin();
    userbt1.attach(rise,RISE);
	userbt1.attach(&test,&Test::event,FALL);
	userbt1.enable(FALL_RISING);	

}


int main(void)
{
    setup();
    while(1)
    {
        ;
    }
}




