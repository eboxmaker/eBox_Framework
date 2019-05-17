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
#include "EventGpio.h"
#include "EventVar.h"
#include "ringbuf.h"
#include "bsp_ebox.h"

/**
	*	1	��������Ҫ����appsĿ¼�µ�EventVar.h , EventManagerģ��
	*	2	��������ʾ��Var�¼���Ӧ���ֱ�Ϊ��󣬱�С���仯�����¼�
	*
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"EventVar example"
#define EXAMPLE_DATE	"2019-05-17"

void up()
{
    UART.println("var ����");
}
void down()
{
    UART.println("var ����");
}void change()
{
    UART.println("var �仯");
}

uint8_t volume = 0;
EventVar<uint8_t> var(&volume);

EventManager event_manager;
void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);    
   
   
    var.event_changed = change;
    var.event_nag_edge = down;
    var.event_pos_edge = up;
    
    event_manager.add(&var);
}
uint32_t last;
uint8_t flag = 0;
int main(void)
{
    setup();

    while(1)
    {
        event_manager.loop();
        delay_ms(1);
        if(millis() - last > 1000)
        {
            last = millis();
            if(flag==0)
                volume++;
            else
                volume--;
            if(volume >= 3)
                flag = 1;
            else if (volume == 0)
                flag = 0;
        }
        
    }
}
