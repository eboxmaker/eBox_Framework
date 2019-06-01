#include "gt30l32s4w.h"
#include "gui_text.h"


extern FontLib font;


bool (*extern_font_api)(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info);

/*********************************************************************
*
*       Graphic text
*
**********************************************************************
*/
void Graphic::attach( bool (*api)(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info))
{
    extern_font_api = api;
}
void Graphic::set_font(const GUI_FONT *font)
{
    this->current_font = (GUI_FONT *)font;

}
void Graphic::set_text_style(uint8_t style)
{
    this->text_style = style;
}
void Graphic::set_text_mode(GuiDrawMode mode)
{
    text_mode = mode;
}


void Graphic::set_font_select(FontSelect_t select){
    
    switch((uint8_t)select)
    {
        case FONT_INNER:
            text_font_ascii_extern_enable = false;
            text_font_hz_extern_enable = false;
            break;
        case FONT_ALL_EXTERN:
            text_font_ascii_extern_enable = true;
            text_font_hz_extern_enable = true;
            break;
            
        case FONT_ONLY_ASCII_EXTERN:
            text_font_ascii_extern_enable = true;
            text_font_hz_extern_enable = false;
            break;
            
        case FONT_ONLY_HZ_EXTERN:
            text_font_ascii_extern_enable = false;
            text_font_hz_extern_enable = true;
            break;
            
        default:
            text_font_ascii_extern_enable = false;
            text_font_hz_extern_enable = false;
            break;

    }
}

void Graphic::set_font_ascii_extern(uint8_t font_id){
    
    text_extern_font_ascii_id = font_id;
}
void Graphic::set_font_hz_extern(uint8_t font_id){
    
    text_extern_font_hz_id = font_id;
}

void Graphic::set_text_auto_reline(uint8_t enable)
{
    text_auto_reline = enable;
}

////解码//////////////////////////////
void Graphic::char_index_of_font(uint16_t code, const GUI_FONT_PROP **font_list, uint16_t *index)
{
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
            pList = pList->pNext;
        }
    }


}

void Graphic::disp_index(const GUI_FONT_PROP *font_list, uint16_t index)
{
    uint32_t count, row, col, mask;
    uint8_t tmp;
    const GUI_CHARINFO *pCharInfo;
    uint8_t byte_per_line;
    if((font_list == NULL) || (index > (font_list->Last - font_list->First + 1)))return;
    pCharInfo = &font_list->paCharInfo[index];
    byte_per_line = pCharInfo->BytesPerLine;
    if( text_auto_reline )
        if(cursor_x + pCharInfo->XSize > _width)cursor_x = 0, cursor_y += current_font->YSize;
    for(row = 0; row < current_font->YSize; row++)
    {
        for( count = 0; count < byte_per_line; count++)
        {
            tmp = pCharInfo->pData[byte_per_line * row + count];
            for(mask = 0x80, col = 0; col < 8 ; mask >>= 1, col++)
            {
                switch(text_mode)
                {
                    case GuiDrawMode::Normal:
                    if(mask & tmp)
                        draw_pixel(cursor_x, cursor_y);
                    else
                        draw_pixel(cursor_x, cursor_y, back_color);
                    break;
                case GuiDrawMode::Xor:
                    if(mask & tmp)
                        draw_pixel(cursor_x, cursor_y);
                    break;
                case GuiDrawMode::Trans:
                    if(mask & tmp)
                        draw_pixel(cursor_x, cursor_y);
                    break;
                case GuiDrawMode::Reverse:
                    if(mask & tmp)
                        draw_pixel(cursor_x, cursor_y, back_color);
                    else
                        draw_pixel(cursor_x, cursor_y);
                    break;

                }
                cursor_x++;
            }
        }
        cursor_x -= byte_per_line * 8;
        cursor_y++;
    }
    cursor_y -= current_font->YDist;
    cursor_x += pCharInfo->XSize;
}
#include "ebox_mem.h"
uint8_t Graphic::get_char_xlength(uint16_t ch,GUI_FONT *font)
{
    const GUI_FONT_PROP *font_list;
    uint16_t index;
    if(ch < 0x7e)    {
//        char_index_of_font(ch, &font_list, &index);
//        disp_index(font_list, index);
        if(text_font_ascii_extern_enable == false )//ASCII使用内部
        {
            char_index_of_font(ch, &font_list, &index);
            return font_list->paCharInfo[index].XSize;
        }
        else
        {
            return char_info.XSize;
        }
    }else{

        if(text_font_hz_extern_enable ==  false)
        {
            char_index_of_font(ch, &font_list, &index);
            return font_list->paCharInfo[index].XSize;

        }else
        {
            return char_info.XSize;
        }


    }
}

void Graphic::disp_char(uint16_t ch)
{
    const GUI_FONT_PROP *font_list;
    uint16_t index;
    if(ch == '\n')
    {
        set_cursor(0, cursor_y + current_font->YSize);
        return;
    }
    if(ch == '\r')
    {
        set_cursor(0, cursor_y);
        return;
    }
    
    
    if(ch < 0x7e)    {
//        char_index_of_font(ch, &font_list, &index);
//        disp_index(font_list, index);
        if(text_font_ascii_extern_enable == false )//ASCII使用内部
        {
            char_index_of_font(ch, &font_list, &index);
            disp_index(font_list, index);
        }
        else
        {
            extern_font_api(ch,text_extern_font_ascii_id,&char_info);
            if(cursor_x + char_info.XSize > _width ){
                cursor_y += char_info.YSize;
                cursor_x = 0;
            }
            drawBitmap(cursor_x,cursor_y,char_info.pData,char_info.XSize,char_info.YSize,0xff);
            ebox_free(char_info.pData);
            cursor_x += char_info.XSize;
        }
    }else{

        if(text_font_hz_extern_enable ==  false)
        {
            char_index_of_font(ch, &font_list, &index);
            disp_index(font_list, index);

        }else
        {

            extern_font_api(ch,text_extern_font_hz_id,&char_info);
            
            if(cursor_x + char_info.XSize > _width ){
                cursor_y += char_info.YSize;
                cursor_x = 0;
            }
            drawBitmap(cursor_x,cursor_y,char_info.pData,char_info.XSize,char_info.YSize,0xff);
            ebox_free(char_info.pData);
            cursor_x += char_info.XSize;
        }


    }
}
void Graphic::disp_char_at(uint16_t ch, int16_t x, int16_t y)
{
    set_cursor(x, y);
    disp_char(ch);
}
void Graphic::disp_chars(uint16_t ch, uint16_t count)
{
    while(count--)
        disp_char(ch);
}
void Graphic::disp_string(const char *str)
{
    uint16_t ch = 0;
    while(*str)
    {
        if(*str < 0x7e)//是字母
            disp_char(*str++);
        else//汉字
        {
            ch = (*str++) << 8;
            ch += *str++;
            disp_char(ch);
        }
    }
}
void Graphic::disp_string_at(const char *str, int16_t x, int16_t y)
{
    set_cursor(x, y);
    disp_string(str);
}
void Graphic::printf(const char *fmt, ...)
{
    char buf[256];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    ebox_vsnprintf(buf, 256, fmt, va_params);
    va_end(va_params);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//是字母
            disp_char(buf[i++]);
        else//汉字
        {
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp << 8) + buf[i++];
            disp_char(tmp);
        }
    }

}

void Graphic::printf(int16_t x, int16_t y, const char *fmt, ...)
{
    char buf[256];
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    ebox_vsnprintf(buf, 256, fmt, va_params);
    va_end(va_params);
    set_cursor(x, y);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//是字母
            disp_char(buf[i++]);
        else//汉字
        {
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp << 8) + buf[i++];
            disp_char(tmp);
        }
    }
}

uint16_t Graphic::get_string_xlength(const char *fmt, ...)
{
    char buf[256];
    uint16_t strXPixelSize = 0;
    uint8_t i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    ebox_vsnprintf(buf, 256, fmt, va_params);
    va_end(va_params);
    while(buf[i] != '\0')
    {
        if(buf[i] < 0x7e)//是字母
            strXPixelSize += get_char_xlength(buf[i++],current_font);
        else//汉字
        {
            uint16_t tmp;
            tmp = buf[i++];
            tmp = (tmp << 8) + buf[i++];
            strXPixelSize += get_char_xlength(tmp,current_font);
        }
    }
    return strXPixelSize;
}
