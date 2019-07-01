#ifndef __GUI_BASE_H
#define __GUI_BASE_H
#include "wstring.h"
#include "ebox_core.h"

DECLARE_ENUM(GuiType)
Button = 0,
List ,
SideBar ,
Text ,
Page,
Menu,
END_ENUM();


DECLARE_ENUM(GuiDockMode)
Left = 0,
Mid,
Right
END_ENUM();


DECLARE_ENUM(GuiOutlineMode)
None = 0,
Surround
END_ENUM();

    
    
    
class GuiBase:public Object
{
    
    public:
        GuiBase(){};
        GuiType type;
        virtual ~GuiBase(){};
    
};
class GuiMessage:public Object
{
    public:
        GuiMessage(){name = "gui msg";}
        String  str;
        int     intValue;
        bool    selected;
};
class Component:public GuiBase
{
    public:
        virtual ~Component(){};
        virtual void create() = 0;
        virtual void show()= 0;
        virtual void hide()= 0;
};
class ActivityComponent:public Component
{
    public:
        ActivityComponent()
        {
            click = NULL;
            selected = false;
        }
        virtual ~ActivityComponent(){};
        virtual void set_select(bool state) = 0;
        void (*click)();
   
    public:
        bool    selected;


};

#endif

