#ifndef __IOEVENT_H
#define __IOEVENT_H
#include "ebox_core.h"
#include "eventManager.h"

/** ����EventGpio���󣬲������¼��ص������ߵ�ƽ���͵�ƽ�������أ�
  *�½��أ��������ͷţ�����.����Ҫ��Ӧ���¼�����Ҫ����
  * ʹ�ó����¼����Զ����õ����¼����ҳ��������󲻴����ͷ��¼�������Ӱ�������غ��½����¼�
  * ʹ�ó����¼������Ժ��ͷ��¼���ϣ���û�д��������¼���ʱ���ͷ��¼��ᱻִ��
  
  *����֧�ֵ��δ���Single����������Continueģʽ�������������¼����������
*/


//����˲�ʱ��ms
#define IO_EDGE_FILTER_COUNTS   0//Ĭ�ϲ�ʹ������˲�
//�����¼������¼�
#define LONG_PRESS_COUNTS       2000//2000ms
//�����¼������������
#define LONG_PRESS_INTERVAL       500//2000ms



class EventGpio : public Event
{
 public:
    typedef enum {
        Single = 0,
        Continue = 1
    }LongPressType_t;
    
    LongPressType_t long_press_type;
    

public:
    EventGpio(Gpio *io,uint8_t first_state,String _name = "null")
    {
        this->io = io;
        this->first_state = first_state;
        state = first_state;
        click_pushed = 0;
        click_released = 0;
        long_pressed = 0;
        
        event_high = NULL;
        event_low = NULL;
        event_pos_edge = NULL;
        event_neg_edge = NULL;
        event_click = NULL;
        event_release = NULL;
        event_long_press = NULL;
        long_press_type = Continue;
        this->name = _name;

    }
    void begin();
    virtual void loop(void);
    void (*event_high)(Object *sender);
    void (*event_low)(Object *sender);
    void (*event_pos_edge)(Object *sender);
    void (*event_neg_edge)(Object *sender);
    void (*event_click)(Object *sender);
    void (*event_release)(Object *sender);
    void (*event_long_press)(Object *sender);


private:
    Gpio *io;
    uint8_t state;
    uint32_t last_time;
    uint16_t long_pressed;
    uint8_t click_pushed;
    uint8_t click_released;
    uint8_t first_state;
    uint8_t changed;
    uint16_t long_press_triger_time;

} ;


#endif
