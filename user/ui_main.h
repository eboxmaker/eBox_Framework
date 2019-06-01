#ifndef __UI_MAIN_H
#define __UI_MAIN_H


#include "gui_page.h"


class MainPage:public GuiPage
{
    
public:
    MainPage(String name);
    virtual ~MainPage();
    virtual void create();
//    virtual void cancel();

    virtual void loop();
    virtual void event(Object *sender,GuiMessage *msg);
};

extern MainPage *pageMain;

#endif

