#include "gui_list.h"


GuiList::~GuiList(){ 
    ebox_printf("GuiList %s free\r\n",name.c_str());
};
void GuiList::create()
{
    show();
}
void GuiList::show()
{
    uint32_t color ;
    int16_t x1 = x + len -1;
    int16_t y1 = y + hight - 1;
    color = _gpu->color;
    _gpu->fill_rect(x,y,x1,y1,_gpu->back_color);
//    _gpu->draw_rect(x,y,x1,y1,RGB565::Dimgray);
    _gpu->draw_rect(x,y,x1,y1,color);

    
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);

    uint16_t slen = _gpu->get_string_xlength(str[index]);
    int16_t startx = (len - slen)/2 + x;
    int16_t starty = (hight - _gpu->current_font->YSize)/2 + y;
    _gpu->printf(startx,starty,str[index]);
    
    if(selected)
    {
//        color = RGB565::LightGray;
        color = _gpu->color;
    }
    else
    {
        color = _gpu->back_color;
    }
    _gpu->draw_rect(x+2,y+2,x1-2,y1-2,color);
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
    uint32_t color ;
    if(selected)
    {
//        color = RGB565::LightGray;
        color = _gpu->color;
    }
    else
    {
        color = _gpu->back_color;
    }
    _gpu->draw_rect(x+2,y+2,x1-2,y1-2,color);

}

int16_t GuiList::index_get()
{
    return index;
}
void GuiList::index_set(int16_t value)
{
    index = value;
    show();
}
bool GuiList::index_next()
{
    if(index < count - 1)
    {      
        index++;
        show();
        return true;
    }
    return false;
}
bool GuiList::index_previous()
{
    if(index > 0)
    {
        index--;
        show();
        return true;
    }
    return false;

}

void GuiList::update_item_index()
{
    show();
}
char *GuiList::get_value()
{
    return str[index];
}

//void GuiList::setText(int value)
//{
//    String str(value);
//    text[0] = str;
//    
//}
