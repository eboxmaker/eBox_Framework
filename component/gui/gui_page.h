#ifndef __GUI_PAGE_H
#define __GUI_PAGE_H
#include "graphic.h"
#include "gui_base.h"
#include "list.h"

class GuiPage :public GuiBase
{
    public :
        GuiPage(String name)
        {
            this->name = name;
            last_selection = selection = 0;
        };
        
        GuiPage *father;
        
        virtual void create();
        virtual void cancel();
        virtual void event(Object *sender,GuiMessage *msg) = 0;
        virtual void show();
        virtual void hide();
        virtual void loop();
        virtual void regedit(ActivityComponent *object);
        virtual void regedit(GuiBase *object);
        virtual GuiBase *get_selected_object();
        virtual void update_select();
    public:
        List activityList;
        List baseList;
        int selection;
        int last_selection;

};
#endif 
