#ifndef __GUI_SIDEBAR_H
#define __GUI_SIDEBAR_H

#include "graphic.h"
#include "gui_base.h"
class GuiSideBar :public Component
{

        
    public:
        GuiSideBar(int16_t x,int16_t y,int16_t bar_len,int16_t bar_width,
                   bool rotation = 0,
                   int16_t max = 10
                       
                   )
        {
            this->x = x;
            this->y = y;
            this->len = bar_len;
            this->width = bar_width;
            
            orientation = rotation;
            this->max = max;
            prograss = 0;
        }
        virtual void create();
        virtual void show();
        virtual void hide();

        //侧变栏信息。只有初始化的时候才直接
        bool orientation;
        int16_t max;
        int16_t prograss;
        
        void set_orientation(int16_t value);
        void set_max(int16_t value);
        void set_prograss(int16_t value);

    private:
        int16_t x;
        int16_t y;
        int16_t len;
        int16_t width;
    
        int16_t slider_len;
        
        void draw_slider(uint32_t color);
        void clear_slider();

};

#endif

