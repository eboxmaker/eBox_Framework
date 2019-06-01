#include "gui_menu.h"

void GuiMenu::create()
{
    _gpu->fill_rect(x,y,+len,y+hight,_gpu->back_color);
    for(int i = 0; i < max; i++)
    {
        if((i + index) >= current_menu->count)
            break;
        _gpu->printf(x,y+i*25,"%s",current_menu->items[i + index].title);
    }
    _gpu->printf(x - 10,y,">");

}
void GuiMenu::show()
{
    _gpu->fill_rect(x,y,+len,y+hight,_gpu->back_color);
    for(int i = 0; i < max; i++)
    {
        if((i + index) >= current_menu->count)
            break;
        _gpu->printf(x,y+i*25,"%s",current_menu->items[i + index].title);
    }
    _gpu->printf(x - 10,y,">");
}
void GuiMenu::hide()
{
    _gpu->fill_rect(x,y,+len,y+hight,_gpu->back_color);

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
    index = value;
    update_item_index();
}

bool GuiMenu::index_next()
{
    if(index < current_menu->count - 1)
    {
        index++;
        update_item_index();
        return true;
    }
    return false;
}
bool GuiMenu::index_previous()
{
    if(index > 0)
    {
        index--;
        update_item_index();
        return true;
    }
    return false;
}
void GuiMenu::update_item_index()
{
    
    _gpu->fill_rect(x,y,+len,y+hight,_gpu->back_color);
    for(int i = 0; i < max; i++)
    {
        if((i + index) >= current_menu->count)
            break;
        _gpu->printf(x,y+i*25,"%s",current_menu->items[i + index].title);
    }
}
