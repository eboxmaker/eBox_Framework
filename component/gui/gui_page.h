#ifndef __GUI_PAGE_H
#define __GUI_PAGE_H
#include "graphic.h"
#include "gui_base.h"
#include "LinkedList.h"

class GuiPage :public Component
{
    public :
        GuiPage(String name)
        {
            this->x = 0;
            this->y = 0;
            this->len = 256;
            this->hight = 64;
            
            this->name = name;
            last_index = index = 0;
            type = GuiType::Page;
            outline = GuiOutlineMode::None;
        };
        
        GuiPage(int16_t x,int16_t y,int16_t len,int16_t hight,
                String name)
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;

            this->name = name;
            last_index = index = 0;
            type = GuiType::Page;
            outline = GuiOutlineMode::Surround;

        };       
        GuiPage *father;
        
        virtual void create();
        virtual void cancel();
        virtual void event(Object *sender,GuiMessage *msg) = 0;
        virtual void show();
        virtual void hide();
        virtual void loop();
        virtual void Register(ActivityComponent *object);
        virtual void Register(Component *object);
        virtual ActivityComponent *get_selected_object();
        
        
        
        int16_t index_get();
        void index_set(int16_t value);
        bool index_next();
        bool index_previous();

    public:
        LinkedList<ActivityComponent*> activityList;
        LinkedList<Component*> componentList;
    
        int index;
        int last_index;
        void update_index();
    
    private:
        int16_t x;
        int16_t y;
        int16_t len;
        int16_t hight;
        GuiOutlineMode outline;

};
#endif 
