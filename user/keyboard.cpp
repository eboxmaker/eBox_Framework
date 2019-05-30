#include "bsp_ebox.h"


#include "keyboard.h"

static EventGpio btn(&PA8, 1,"btn1");
EventManager manager;

Keyboard::Keyboard()
{
    
    
    add(&btn);
}
void Keyboard::onEvent(Object *sender)
{

}
void Keyboard::begin()
{
    
    
//    btn.event_release = release;
//    btn.event_long_press = long_press;
    btn.long_press_type = EventGpio::Single;

    btn.begin();
    manager.add(&btn);

}

Keyboard keyboard;

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
void attach(void (*ptr)(Object *sender))
{
    btn.event_click = ptr;

}
void keyboar_init()
{
    attach(keyboard.onEvent);
}