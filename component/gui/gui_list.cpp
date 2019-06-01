#include "gui_list.h"

//static const uint8_t itemValueTable[10] = {0,1,2,3,4,5,6,7,8,9};
GuiList::~GuiList(){ 
    ebox_printf("GuiList %s free\r\n",name.c_str());
};
void GuiList::create()
{

    int16_t x1 = x + len -1;
    int16_t y1 = y + hight - 1;
    _gpu->draw_rect(x+2,y+2,x1-2,y1-2,RGB565::Dimgray);

    
    _gpu->set_font(&GUI_FontHZ16X16);
    _gpu->set_text_mode(GuiDrawMode::Normal);

    String str(value[index]);
    uint16_t slen = _gpu->get_string_xlength(str.c_str());
    int16_t startx = (len - slen)/2 + x;
    int16_t starty = (hight - _gpu->current_font->YSize)/2 + y;
    _gpu->printf(startx,starty,str.c_str());
}
void GuiList::show()
{
    create();
}
void GuiList::hide()
{
    _gpu->fill_rect(x,y,+len,y+hight,_gpu->back_color);

}
void GuiList::set_select(bool state)
{
    selected = state;
    int16_t x1 = x + len -1;
    int16_t y1 = y + hight - 1;
    if(selected)
    {
        _gpu->draw_h_line(x,y,x1,RGB565::LightGray);
        _gpu->draw_h_line(x,y1,x1,RGB565::LightGray);
        _gpu->draw_v_line(x,y,y1,RGB565::LightGray);
        _gpu->draw_v_line(x1,y,y1,RGB565::LightGray);
    }
    else
    {
        _gpu->draw_h_line(x,y,x1,_gpu->back_color);
        _gpu->draw_h_line(x,y1,x1,_gpu->back_color);
        _gpu->draw_v_line(x,y,y1,_gpu->back_color);
        _gpu->draw_v_line(x1,y,y1,_gpu->back_color);
    }
}
void GuiList::update_value()
{
    _gpu->set_font(&GUI_FontHZ16X16);
    _gpu->set_text_mode(GuiDrawMode::Normal);

    String str(value[index]);

    uint16_t slen = _gpu->get_string_xlength(str.c_str());
    int16_t startx = (len - slen)/2 + x;
    int16_t starty = (hight - _gpu->current_font->YSize)/2 + y;
    _gpu->printf(startx,starty,str.c_str());
}
uint8_t GuiList::get_value()
{
    return value[index];
}

//void GuiList::setText(int value)
//{
//    String str(value);
//    text[0] = str;
//    
//}
