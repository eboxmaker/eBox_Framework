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
#include "bsp_ebox.h"
#include "EventVar.h"

/**
	*	1	��������Ҫ����appsĿ¼�µ�EventGpio , eventVar,EventManagerģ��
	*	2	��������ʾ��IO�¼���Ӧ���ֱ�Ϊ�͵�ƽ���ߵ�ƽ���½��أ������أ��������ͷţ�����
	*	3	�߶�ƽ���͵�ƽ��������������������5ms����ͨ���޸�EvenGpio.h�е�IO_EDGE_FILTER_COUNTS�޸�
	* 4	����ʱ����õ����¼����ҳ��������󲻴����ͷ��¼�������Ӱ�������غ��½����¼�
	* 5 ���û�г��������½���=������������=�ͷ�
	*
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"EventVar example"
#define EXAMPLE_DATE	"2019-05-17"
/** ����EventGpio���󣬲������¼��ص������ߵ�ƽ���͵�ƽ�������أ�
  *�½��أ��������ͷţ�����.����Ҫ��Ӧ���¼�����Ҫ����
	*/
// ʹ�ó����¼����Զ����õ����¼����ҳ��������󲻴����ͷ��¼�������Ӱ�������غ��½����¼�
// ʹ�ó����¼������Ժ��ͷ��¼���ϣ���û�д��������¼���ʱ���ͷ��¼��ᱻִ��

uint8_t volume = 0;
EventVarUint8 var(&volume,"volume");

void up(Object *sender)
{
    EventVarUint8 *ptr = (EventVarUint8 *)sender; 
    UART.print("var:");
    UART.print((uint8_t)(*ptr->var));
    UART.println(" ����");
}
void down(Object *sender)
{
    EventVarUint8 *ptr = (EventVarUint8 *)sender; 
    UART.print("var:");
    UART.print((uint8_t)(*ptr->var));
    UART.println(" ����");
}void change(Object *sender)
{
    UART.println("var �仯");
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    
    var.event_changed = change;
    var.event_nag_edge = down;
    var.event_pos_edge = up;
    
    manager.add(&var);
    manager.print_list(UART);
}
uint32_t last;
uint8_t flag = 0;
int main(void)
{
    setup();

    while(1)
    {
        manager.loop();
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
