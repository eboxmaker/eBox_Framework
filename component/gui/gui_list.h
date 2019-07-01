#ifndef __GUI_LIST_H
#define __GUI_LIST_H

#include "graphic.h"
#include "gui_base.h"


class GuiList :public ActivityComponent
{

public:

    public:
        GuiList(int16_t x,int16_t y,int16_t len,int16_t hight,
                char **str,uint16_t str_size)
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
            this->str = str;
            this->count = str_size;
            index = 0;
            name = "";
            type = GuiType::List;
            font = &GUI_Font16_ASCII;
            text_mode = GuiDrawMode::Normal;
        }
        
        virtual ~GuiList();
        virtual void create();
        virtual void show();
        virtual void hide();
        virtual void set_select(bool state);
            
        void setIndex();
        char *get_value();
        
        
        //�˵�ѡ������
        int16_t index;
        int16_t index_get();
        void index_set(int16_t value);
        bool index_next();
        bool index_previous();
        void update_item_index();
        
    public:
        int16_t x,y;
        int16_t len,hight;
        char **str;//��Ŀ��ʾֵ
        int16_t count;//list�ܹ��ж�����

        const GUI_FONT *font;
        GuiDrawMode text_mode;
};

#endif

