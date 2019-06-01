#include "gui_textview.h"


void GuiTextView::create()
{
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);
    _gpu->printf(x,y,text.c_str());
    
}
void GuiTextView::show()
{
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);
    _gpu->printf(x,y,text.c_str());
}
void GuiTextView::hide()
{
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);
    int len = _gpu->get_string_xlength(text.c_str());
    _gpu->fill_rect(x,y,x +len,font->YSize);

}