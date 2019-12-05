#include "EventGpio.h"

void EventGpio::begin()
{
    if(this->first_state == 0)
        io->mode(INPUT_PD);
    else
        io->mode(INPUT_PU);

}
void EventGpio::loop()
{
    if( io->read() != state)    // ���״̬�仯
    {
        if(changed == 0)//���û�б������һ�α仯���Ͳ���һ���仯�ı��
        {
            last_time = millis();
            changed = 1;
        }
        else//����Ѿ�����ǡ�
        {
            //����������˲�ʱ�䣬
            if(( millis() - last_time ) > IO_EDGE_FILTER_COUNTS)
            {
                if(state == 1)//�ҳ�ʼ̬Ϊ1����˵�����½���
                {
                    state = 0;//��IO״̬��Ǹ�Ϊ��ǰIO״̬
                    if(event_neg_edge != NULL)
                        event_neg_edge(this);
                }
                else//��ʼ̬Ϊ0����������
                {
                    state = 1;//��IO״̬��Ǹ�Ϊ��ǰIO״̬
                    if(event_pos_edge != NULL)
                        event_pos_edge(this);
                }
                changed = 0;//ִ����ص����������仯������
            }

        }
    }
    //������˲��ڷ�����ë�̣�Ҫ���ë�̲�����״̬
    if(( millis() - last_time ) > IO_EDGE_FILTER_COUNTS)
    {
        if(changed == 1 && io->read() == first_state)
            changed = 0;

    }
    

    //stateΪ�˲����IO״ֵ̬
    if(state == first_state )//����ͳ�ʼ̬һ��
    {
        if(click_pushed && (long_pressed == 0))// �Ҽ�⵽�����£���û�д���long_press�¼���������ͷ��¼�
        {
            click_released = 1;//����ͷ��¼�������
            if(event_release !=  NULL)
                event_release(this);
        }
        click_pushed = 0;//�����Ѿ����ͷţ�������µ�״̬


    }
    else//stateΪ�˲����IO״ֵ̬������ͳ�ʼ̬��һ��
    {
        if(millis() - last_time > IO_EDGE_FILTER_COUNTS)//��֤�˲�ʱ��
        {
            if(event_long_press !=  NULL)//ʹ��long_press������click�����ʹ��long_press�򴥷�long_press֮�󲻻��ٴ���release
            {
                if(millis()  - last_time < LONG_PRESS_COUNTS)
                {
                    click_pushed = 1;
                    long_pressed = 0;
                    long_press_triger_time = millis();
                }
                else //stateΪ�˲����IO״ֵ̬������ͳ�ʼ̬��һ�����ҳ����˳���������������֮ǰûִ�й������¼�
                {
                    if( long_press_type == Single)
                    {
                        if(long_pressed == 0)
                        {
                            {
                                long_pressed = 1;
                                if(event_long_press !=  NULL)
                                    event_long_press(this);
                            }
                        }
                    }
                    else
                    {
                        if( millis()  - long_press_triger_time  > LONG_PRESS_INTERVAL)
                        {
                            long_press_triger_time = millis();
                            //�Ұ����ǰ��µ�״̬����ִ�г����¼�
                            {
                                long_pressed = 1;
                                if(event_long_press !=  NULL)
                                    event_long_press(this);
                            }
                        }
                    }

                    click_released = 0;//����ִ���ͷ��¼�
                }
            }
            else//��ʹ��long_press
            {
                if(click_pushed == 0)//stateΪ�˲����IO״ֵ̬������ͳ�ʼ̬��һ������֮ǰûִ�й������¼�����ִ�е����¼�
                {
                    click_pushed = 1;
                    if(event_click !=  NULL)
                        event_click(this);
                }

            }

        }

    }
    if(io->read() == 1){
        if(event_high != NULL)
            event_high(this);
    }
    else{
        if(NULL != event_low)
            event_low(this);
    }
}
