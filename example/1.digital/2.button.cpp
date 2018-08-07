/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
	*					 2018-8-1	ͨ������bsp������Ӳ���˿ڣ��������̿����ڲ�ͬƽ̨������
  * Copyright 2016 shentq. All Rights Reserved.         
  ******************************************************************************
 */

#include "ebox.h"
#include "button.h"
#include "bsp_ebox.h"

/**
	*	1	��������Ҫ����eDriveĿ¼�µ�Buttonģ��
	*	2	��������ʾ�˰���  ���£����𣬳��� ���ֲ���
	*/


/* ���������������̷������� */
#define EXAMPLE_NAME	"Button example"
#define EXAMPLE_DATE	"2018-08-01"


// ����btn����IO��������ģʽ
Button btn(&BtnPin,1);

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    btn.begin();
    LED1.mode(OUTPUT_PP);
    LED2.mode(OUTPUT_PP);
    LED3.mode(OUTPUT_PP);
}



int main(void)
{
    setup();
    while(1)
    {
        btn.loop();	//��ȡ������ǰ״̬����Ҫ��ʱ����ִ��
			
        if(btn.click())				// �����Ƿ���
        {
            LED1.toggle();
            UART.printf("\r\nclick event!");
        }
        if(btn.release())			// �����ͷ�
        {
            LED2.toggle();
            UART.printf("\r\nrelease event!");
        }
        if(btn.pressed_for(2000, 1)) //�������룬ִ��1��
        {
            LED3.toggle();
            UART.printf("\r\nlong press event!");
        }
				
//				if(btn.pressed_for(2000, 5)) //�������룬ִ��5��
//        {
//            LED3.toggle();
//            UART.printf("\r\nlong press event!");
//        }
    }
}



