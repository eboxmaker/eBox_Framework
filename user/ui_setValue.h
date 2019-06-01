#ifndef __UI_SETVALUE_H
#define __UI_SETVALUE_H


#include "gui_page.h"


class SetValuePage:public GuiPage
{
    
public:
    SetValuePage(String name);
    virtual ~SetValuePage();
    virtual void create();

    virtual void loop();
    virtual void event(Object *sender,GuiMessage *msg);
};

extern SetValuePage *pageSetValue;

#endif
