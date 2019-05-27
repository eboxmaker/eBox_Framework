#ifndef __GUI_SIDEBAR_H
#define __GUI_SIDEBAR_H

#include "graphic.h"
#include "gui_base.h"
class GuiSideBar :public GuiBase
{

        
    public:
        GuiSideBar(int16_t x,int16_t y,int16_t bar_len,int16_t bar_width)
        {
            this->x = x;
            this->y = y;
            this->len = bar_len;
            this->width = bar_width;
            
            orientation = 0;
            max = 10;
            prograss = 0;
        }
        void set_max(int16_t value);
        void set_prograss(int16_t value);
        void set_orientation(int16_t value);
        void draw();

    private:
        int16_t x;
        int16_t y;
        int16_t len;
        int16_t width;
    
        bool orientation;
        int16_t slider_len;
        int16_t max;
        int16_t prograss;
        
        void draw_slider(uint32_t color);
        void clear_slider();

};

#endif