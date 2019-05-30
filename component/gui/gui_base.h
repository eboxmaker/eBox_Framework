#ifndef __GUI_BASE_H
#define __GUI_BASE_H
#include "wstring.h"
#include "ebox_core.h"
class GuiBase:public Object
{
    
    public:
        GuiBase(){click = NULL;};
        
    virtual ~GuiBase(){};
    virtual void create() = 0;
        
    void (*click)();
    
};
class GuiMessage:public Object
{
public:
    GuiMessage(){name = "gui msg";}
    String  str;
    int     intValue;
    bool    selected;
};
class ActivityComponent:public GuiBase
{
    public:
    virtual void set_select(bool state) = 0;

};

#endif