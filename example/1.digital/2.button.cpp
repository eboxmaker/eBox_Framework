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
#include "EventManager.h"
#include "bsp_ebox.h"

/**
	*	1	��������Ҫ����appsĿ¼�µ�EventGpio ,EventManagerģ��
	*	2	��������ʾ��IO�¼���Ӧ���ֱ�Ϊ�͵�ƽ���ߵ�ƽ���½��أ������أ��������ͷţ�����
	*	3	�߶�ƽ���͵�ƽ��������������������5ms����ͨ���޸�EvenGpio.h�е�IO_EDGE_FILTER_COUNTS�޸�
	* 4	����ʱ����õ����¼����ҳ��������󲻴����ͷ��¼�������Ӱ�������غ��½����¼�
	* 5 ���û�г��������½���=������������=�ͷ�
	*
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"EventGPIO example"
#define EXAMPLE_DATE	"2018-08-02"


/** ����EventGpio���󣬲������¼��ص������ߵ�ƽ���͵�ƽ�������أ�
  *�½��أ��������ͷţ�����.����Ҫ��Ӧ���¼�����Ҫ����
	*/
// ʹ�ó����¼����Զ����õ����¼����ҳ��������󲻴����ͷ��¼�������Ӱ�������غ��½����¼�
// ʹ�ó����¼������Ժ��ͷ��¼���ϣ���û�д��������¼���ʱ���ͷ��¼��ᱻִ��
EventGpio btn(&PA8, 1,"btn1");
EventManager manager;

// �½��ؼ��
void neg(Object *sender)
{
    UART.println("��⵽�½����ź�");
}
// �����ؼ��
void pos(Object *sender)
{
    UART.println("��⵽�������ź�");
}
// �ߵ�ƽ�ص�����
void high(Object *sender)
{
    UART.println("��⵽�ߵ�ƽ");
}

// �ߵ�ƽ�ص�����
void low(Object *sender)
{
    UART.println("��⵽�͵�ƽ");
}
// �����ص�����
void click(Object *sender)
{
    UART.println("��⵽��������");
}
// �ͷŻص�����
void release(Object *sender)
{
    if(&btn == sender)
        UART.println("��⵽�����ͷ�");
    EventGpio *newObject = (EventGpio *)sender;
    UART.println(newObject->name);
    UART.println(sender->name);
}
// �����ص�����
void long_press(Object *sender)
{
    UART.println("��⵽����������ʱ����Ӧ����");
}





void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    
    
    btn.event_click = click;
    btn.event_release = release;
    btn.event_long_press = long_press;
    btn.long_press_type = EventGpio::Single;
//    btn.event_high = high;
//    btn.event_low = low;
//    btn.event_neg_edge = neg;
//    btn.event_pos_edge = pos;
    
    btn.begin();
    manager.add(&btn);
    
    manager.print_list(UART);
}
int main(void)
{
    setup();

    while(1)
    {
        manager.loop();
        delay_ms(1);
    }
}
