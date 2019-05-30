#ifndef __KEYBOARD_H
#define __KEYBOARD_H
#include "ebox_core.h"
#include "EventGpio.h"
#include "EventManager.h"
class  Keyboard:public EventManager
{
public:
    Keyboard();
    void begin();

    void onEvent(Object *sender);
    
};
extern Keyboard keyboard;

#endif

