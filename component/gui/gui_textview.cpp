#include "gui_textview.h"

GuiTextView::~GuiTextView()
{
   // ebox_printf("TV(%s) deleted\r\n",name.c_str());
}

void GuiTextView::create()
{
    int16_t slen,startx,starty;
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);
    if(advance_enable == false )
    {
        slen = _gpu->get_string_xlength(text.c_str());
        len = slen;
        _gpu->printf(x,y,text.c_str());

    }
    else
    {
        int16_t x1 = x + len - 1;
        int16_t y1 = y + hight - 1;          
        

        switch(dock)
        {
            case GuiDockMode::Mid:
                slen = _gpu->get_string_xlength(text.c_str());
                startx = (len - slen)/2 + x;

                break;
            case GuiDockMode::Right:
                slen = _gpu->get_string_xlength(text.c_str());
                startx = len  + x - slen;
                break;
            default :
                startx = x ;
                break;
        }
			_gpu->fill_rect(x,y,x1,y1,RGB565::Black);
        switch(outline_mode)
        {
            case GuiOutlineMode::None:
                break;
            case GuiOutlineMode::Surround:
                _gpu->draw_rect(x,y,x1,y1);
                break;
        }

        _gpu->printf(startx,y,text.c_str());

    }

    
}
void GuiTextView::show()
{
    create();
}
void GuiTextView::hide()
{
    _gpu->set_font(font);
    _gpu->set_text_mode(text_mode);

    int slen = _gpu->get_string_xlength(text.c_str());
    _gpu->fill_rect(x,y, x + slen, y + font->YSize, _gpu->back_color);
    
}

void GuiTextView::set_text(String text)
{

    this->text = text;
    create();
}
