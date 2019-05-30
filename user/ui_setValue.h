#ifndef __UI_SETVALUE_H
#define __UI_SETVALUE_H


#include "gui_page.h"


class SetValuePage:public GuiPage
{
    
public:
    SetValuePage(String name);
    ~SetValuePage();
    virtual void create();
    void loop();
    virtual void event(Object *sender,GuiMessage *msg);
};

extern SetValuePage *pageSetValue;

#endif
