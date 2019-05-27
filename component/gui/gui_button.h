#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include "graphic.h"
#include "gui_base.h"
class GuiButton :public GuiBase
{

    public:
        GuiButton(int16_t x,int16_t y,int16_t len,int16_t hight)
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
        }
        void draw();
    
    public:
        int16_t x,y;
        int16_t len,hight;
};

#endif