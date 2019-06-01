#ifndef __GUI_PAGE_H
#define __GUI_PAGE_H
#include "graphic.h"
#include "gui_base.h"
#include "list.h"

class GuiPage :public Component
{
    public :
        GuiPage(String name)
        {
            this->name = name;
            last_index = index = 0;
            type = GuiType::Page;
        };
        
        GuiPage *father;
        
        virtual void create();
        virtual void cancel();
        virtual void event(Object *sender,GuiMessage *msg) = 0;
        virtual void show();
        virtual void hide();
        virtual void loop();
        virtual void regedit(ActivityComponent *object);
        virtual void regedit(Component *object);
        virtual ActivityComponent *get_selected_object();
        
        
        
        int16_t index_get();
        void index_set(int16_t value);
        bool index_next();
        bool index_previous();

    public:
        List activityList;
        List componentList;
    
        int index;
        int last_index;
        void update_index();

};
#endif 
