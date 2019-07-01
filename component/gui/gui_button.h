#ifndef __GUI_BUTTON_H
#define __GUI_BUTTON_H

#include "graphic.h"
#include "gui_base.h"
class GuiButton :public ActivityComponent
{

    public:
        GuiButton(int16_t x,int16_t y,int16_t len,int16_t hight,
                  String name = "NULL")
        {
            this->x = x;
            this->y = y;
            this->len = len;
            this->hight = hight;
            text = "NULL";
            this->name = name;
            type = GuiType::Button;
            font = &GUI_Font16_ASCII;
            text_mode = GuiDrawMode::Trans;

        }
        virtual ~GuiButton();
        virtual void create();
        virtual void show();
        virtual void hide();
        virtual void set_select(bool state);


        void set_text(String str);
//        void btn_down(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
//        void btn_up(int16_t x1, int16_t y1, int16_t x2, int16_t y2);
    public:
        int16_t x,y;
        int16_t len,hight;
        String text;
        bool    pushed;
        const GUI_FONT *font;
        GuiDrawMode text_mode;
};


#endif

