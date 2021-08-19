#ifndef __UI_PASSWORD_H
#define __UI_PASSWORD_H


#include "gui_page.h"


class PwdPage:public GuiPage
{
    
public:
    PwdPage(String name);
    virtual ~PwdPage();
    virtual void create();

    virtual void loop();
    virtual void event(Object *sender,GuiMessage *msg);
public:
    String pwd;
};

extern PwdPage *pagePwd;

#endif

