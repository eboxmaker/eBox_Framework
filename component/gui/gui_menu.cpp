#include "gui_menu.h"

void GuiMenu::create()
{
    show();
}
void GuiMenu::show()
{
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);

    _gpu->fill_rect(x,y,x+len,y+hight,_gpu->back_color);

    for(int i = 0; i < max; i++)
    {
        if((i + index) >= current_menu->count)
            break;
        _gpu->printf(x,y+i*font->YSize,"%d:%s",index + 1 + i ,current_menu->items[i + index].title);
    }
    if(max != 1)
        _gpu->printf(x - 10,y,">");
}
void GuiMenu::hide()
{
        _gpu->fill_rect(x,y,x+len,y+hight,_gpu->back_color);

//    _gpu->set_font(font);
//    
//    for(int i = 0; i < max; i++)
//    {
//        if((i + index) >= current_menu->count)
//            break;
//        uint16_t slen = _gpu->get_string_xlength(current_menu->items[index].title);
//        _gpu->fill_rect(x,y,x+slen,y + font->YSize,_gpu->back_color);
//    }
    
}
void GuiMenu::set_select(bool state)
{
    
}
int16_t GuiMenu::index_get()
{
    return index;
}

void GuiMenu::index_set(int16_t value)
{
    _gpu->set_font(font);
    
//    for(int i = 0; i < max; i++)
//    {
//        if((i + index) >= current_menu->count)
//            break;
//        uint16_t slen = _gpu->get_string_xlength(current_menu->items[index].title);
//        _gpu->fill_rect(x,y,x+slen,y + font->YSize,_gpu->back_color);
//    }
    
    index = value;
    show();
}

bool GuiMenu::index_next()
{
    if(index < current_menu->count - 1)
    {
//        _gpu->set_font(font);
//        
//        for(int i = 0; i < max; i++)
//        {
//            if((i + index) >= current_menu->count)
//                break;
//            uint16_t slen = _gpu->get_string_xlength(current_menu->items[index].title);
//            _gpu->fill_rect(x,y,x+slen,y + font->YSize,_gpu->back_color);
//        }
//    
        
        index++;
        show();
        return true;
    }
    return false;
}
bool GuiMenu::index_previous()
{
    if(index > 0)
    {
        
//        _gpu->set_font(font);
        
//        for(int i = 0; i < max; i++)
//        {
//            if((i + index) >= current_menu->count)
//                break;
//            uint16_t slen = _gpu->get_string_xlength(current_menu->items[index].title);
//            _gpu->fill_rect(x,y,x+slen,y + font->YSize,_gpu->back_color);
//        }
//    

        index--;
        show();
        return true;
    }
    return false;
}
void GuiMenu::update_item_index()
{
    
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);

    _gpu->fill_rect(x,y,x+len,y+hight,_gpu->back_color);
   
    for(int i = 0; i < max; i++)
    {
        if((i + index) >= current_menu->count)
            break;
        _gpu->printf(x,y+i*font->YSize,"%d:%s",index + 1 + i ,current_menu->items[i + index].title);
    }
}
