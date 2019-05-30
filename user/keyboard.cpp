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

// 单击回调函数
void click(Object *sender)
{
    UART.println("检测到按键单击");
}
// 释放回调函数
void release(Object *sender)
{
    if(&btn == sender)
        UART.println("检测到按键释放");
    EventGpio *newObject = (EventGpio *)sender;
    UART.println(newObject->name);
    UART.println(sender->name);
}
// 长按回调函数
void long_press(Object *sender)
{
    UART.println("检测到长按，长按时不响应单击");
}
void attach(void (*ptr)(Object *sender))
{
    btn.event_click = ptr;

}
void keyboar_init()
{
    attach(keyboard.onEvent);
}