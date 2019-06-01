#ifndef __GUI_LIST_H
#define __GUI_LIST_H

#include "graphic.h"
#include "gui_base.h"
class GuiList :public ActivityComponent
{

    public:
        GuiList(int16_t x,int16_t y,int16_t len,int16_t hight)
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
            index = 0;
            name = "";
            type = GuiType::List;
            for(int i = 0; i < 10; i++)
                value[i] = i;
        }
        virtual ~GuiList();
        virtual void create();
        virtual void show();
        virtual void hide();
        virtual void set_select(bool state);
            
        void setIndex();
        uint8_t get_value();
        void update_value();
        
    public:
        int16_t x,y;
        int16_t len,hight;
        uint8_t value[10];//项目显示值
        int16_t count;//list总共有多少项
        int16_t index;
};

#endif

