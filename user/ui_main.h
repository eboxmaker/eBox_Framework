#ifndef __UI_MAIN_H
#define __UI_MAIN_H


#include "gui_page.h"


class MainPage:public GuiPage
{
    
public:
    MainPage(String name);
    ~MainPage();
    virtual void create();
    void loop();
    virtual void event(Object *sender,GuiMessage *msg);
};

extern MainPage *pageMain;

#endif

