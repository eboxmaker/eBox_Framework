#include "gui.h"

/*********************************************************************
*
*       GUI text
*
**********************************************************************
*/
void GUI::set_font(const GUI_FONT *font)
{
    this->current_font = (GUI_FONT *)font;

}
void GUI::set_text_style(uint8_t style)
{
    this->text_style = style;
}
void GUI::set_text_mode(uint8_t mode)
{   
    text_mode = mode;
}
void GUI::set_text_auto_reline(uint8_t enable)
{
    text_auto_reline = enable;
}

////½âÂë//////////////////////////////
void GUI::char_index_of_font(uint16_t code,const GUI_FONT_PROP **font_list,uint16_t *index)
{
    uint16_t tmp;
    uint16_t count;
    const GUI_FONT_PROP *pList;
    pList = current_font->list;
    
    while(pList != NULL)
    {
        if(code >= pList->First && code <= pList->Last)
        {
            *font_list = pList;
            *index = code - pList->First;
            break;
        }
        else
        {
            *font_list = current_font->list;
            *index = 0; 
            pList=pList->pNext;
        }
    }


}

void GUI::disp_index(const GUI_FONT_PROP *font_list,uint16_t index)
{
	uint32_t count, row ,col, mask;
	uint8_t tmp;
    const GUI_CHARINFO *pCharInfo;
    uint8_t byte_per_line;
    if((font_list == NULL) || (index > (font_list->Last - font_list->First + 1)))return;
    pCharInfo = &font_list->paCharInfo[index];
    byte_per_line = pCharInfo->BytesPerLine;
    if( text_auto_reline )
        if(cursor_x + pCharInfo->XSize > _width)cursor_x = 0,cursor_y+=current_font->YSize;
	for(row = 0; row < current_font->YSize; row++){   
        for( count = 0; count < byte_per_line; count++){
            tmp = pCharInfo->pData[byte_per_line * row + count];
            for(mask = 0x80, col = 0; col < 8 ; mask >>= 1, col++){	
                switch(text_mode)
                {
                    case TEXT_MODE_NORMAL:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y);
                        else
                            draw_pixel(cursor_x,cursor_y,back_color);
                        break;
                    case TEXT_MODE_XOR:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y);
                        break;
                    case TEXT_MODE_TRANS:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y);
                        break;
                    case TEXT_MODE_REV:
                        if(mask & tmp)
                            draw_pixel(cursor_x,cursor_y,back_color);
                        else
                            draw_pixel(cursor_x,cursor_y);
                        break;

                }
                cursor_x++;
            }
        }
            cursor_x-=byte_per_line * 8;
            cursor_y++;
    }
    cursor_y-=current_font->YDist;
    cursor_x+=pCharInfo->XSize;
}

void GUI::disp_char(uint16_t ch)
{
    const GUI_FONT_PROP *font_list;
    uint16_t index;
    if(ch == '\n')
    {
        set_cursor(0,cursor_y+current_font->YSize);
        return;
    }
    if(ch == '\r')
    {
        set_cursor(0,cursor_y);
        return;
    }
    char_index_of_font(ch,&font_list,&index);
    disp_index(font_list,index);
}
void GUI::disp_char_at(uint16_t ch,int16_t x,int16_t y)
{
    set_cursor(x,y);
    disp_char(ch);
}
void GUI::disp_chars(uint16_t ch,uint16_t count)
{
    while(count--)
        disp_char(ch);
}
void GUI::disp_string(const char *str)
{
    uint16_t ch = 0;
    while(*str)
    {
        if(*str < 0x7e)//ÊÇ×ÖÄ¸
            disp_char(*str++);
        else//ºº×Ö
        {            
            ch = (*str++)<<8;
            ch += *str++;
            disp_char(ch);        
        }
    }
}
void GUI::disp_string_at(const char *str,int16_t x,int16_t y)
{
    set_cursor(x,y);
    disp_string(str);
}

void GUI::printf(const char *fmt, ...)
{
    char buf[30];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//ÊÇ×ÖÄ¸
            disp_char(buf[i++]);
        else//ºº×Ö
        {        
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp<<8) + buf[i++];
            disp_char(tmp);        
        }
    }
 
}
    
void GUI::printf(int16_t x, int16_t y, const char *fmt, ...)
{
    char buf[30];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    set_cursor(x,y);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//ÊÇ×ÖÄ¸
            disp_char(buf[i++]);
        else//ºº×Ö
        {        
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp<<8) + buf[i++];
            disp_char(tmp);        
        }
    }
}
