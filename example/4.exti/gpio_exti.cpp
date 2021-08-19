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
#include "ebox_exti.h"
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


Exti   userbt1(&BTN1);
//Exti   ex(&BtnPin);

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
    LED1.toggle();
    UART.printf("\r\n rising, xx = %d", xx);
}

void exti_event()
{
    xx++;
    LED1.toggle();
    if(userbt1.read())
        UART.printf("\r\n rise, xx = %d", xx);
    else
        UART.printf("\r\n fall, xx = %d", xx);


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
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    LED1.mode(OUTPUT_PP);
    // �����أ��½��ؾ�����,��ͬһ���жϻص�����
    userbt1.begin();
    userbt1.attach(exti_event);
    userbt1.interrupt(ApiExti::TrigFall,ENABLE);

//    // �����أ��½��ص��ò�ͬ�Ļص�����
//    userbt1.begin();
//    userbt1.attach(rise);
//    userbt1.interrupt(TrigFall,ENABLE);

}


int main(void)
{
    setup();
    while(1)
    {
        //userbt1.soft_triger();
        delay_ms(1000);
    }
}




