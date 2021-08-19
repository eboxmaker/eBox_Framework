#ifndef __UI_MENU_H
#define __UI_MENU_H


#include "gui_page.h"


class MenuPage:public GuiPage
{
    
public:
    MenuPage(String name):GuiPage(name){};
    virtual ~MenuPage(){};
    virtual void create();
    virtual void show();

    virtual void loop();
    virtual void event(Object *sender,GuiMessage *msg);
};

extern MenuPage *pageMenu;

#endif

