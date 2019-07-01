#ifndef __GUI_TEXTVIEW_H
#define __GUI_TEXTVIEW_H

#include "graphic.h"
#include "gui_base.h"

class GuiTextView :public Component
{
public:

    public:

        GuiTextView(int16_t x,int16_t y,
                    String name = "")
        {
            this->x = x;
            this->y = y;
            this->len = 0;
            this->hight = 0;
            this->name = name;
            type = GuiType::Text;
            text = "";
            font = &GUI_Font16_ASCII;
            text_mode = GuiDrawMode::Trans;
            advance_enable = false;
        }   
        
        GuiTextView(int16_t x,int16_t y,int16_t len,int16_t hight,
                    String name = "")
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
            this->name = name;
            type = GuiType::Text;
            text = "";
            font = &GUI_Font16_ASCII;
            text_mode = GuiDrawMode::Trans;
            dock = GuiDockMode::Left;
            outline_mode = GuiOutlineMode::None;
            advance_enable = true;
        }      

        virtual ~GuiTextView();
        virtual void create();
        virtual void show();
        virtual void hide();
        
        void set_text(String text);
            
                    
        
    public:

        String text;//项目显示值
        const GUI_FONT *font;
        GuiDrawMode text_mode;
        GuiOutlineMode outline_mode;
        GuiDockMode   dock;
    private:
        int16_t x,y;
        int16_t len,hight;
        bool    advance_enable;
//            _gpu->set_font(&GUI_FontHZ16X16);
//    _gpu->set_text_mode(DRAW_MODE_TRANS);
};

#endif

