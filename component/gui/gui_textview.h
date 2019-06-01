#ifndef __GUI_TEXTVIEW_H
#define __GUI_TEXTVIEW_H

#include "graphic.h"
#include "gui_base.h"

class GuiTextView :public Component
{

    public:
        GuiTextView(int16_t x,int16_t y)
        {
            this->x = x;
            this->y = y;
            name = "";
            type = GuiType::Text;
            text = "";
            font = &GUI_Font16_ASCII;
            text_mode = GuiDrawMode::Trans;
        }
        virtual ~GuiTextView(){};
        virtual void create();
        virtual void show();
        virtual void hide();
            
            
        uint8_t get_value();
        
        
    public:
        int16_t x,y;
        String text;//项目显示值
        const GUI_FONT *font;
        GuiDrawMode text_mode;
//            _gpu->set_font(&GUI_FontHZ16X16);
//    _gpu->set_text_mode(DRAW_MODE_TRANS);
};

#endif

