#include "ebox_virtual_lcd.h"
#include "ebox_mem.h"
VLcd::VLcd()
{
};

VLcd::~VLcd()
{
    ebox_free(lcd.pixel);
};
void VLcd::begin(uint16_t lcd_w, uint16_t lcd_h)
{
    lcd.width = lcd_w;
    lcd.height = lcd_h;
    lcd.mem_size = sizeof(LcdPixel_t) * lcd.width * lcd.height;
    lcd.pixel = (LcdPixel_t *)ebox_malloc(lcd.mem_size);
    memset(lcd.pixel, 0, lcd.mem_size);

}
int32_t VLcd::position_to_index(int16_t x, int16_t y)
{
    if(x < 0 || x >= lcd.width)return -1 ;
    if(y < 0 || y >= lcd.width)return -1 ;
    return x + y * lcd.width;
}
void VLcd::set_xy(int16_t x, int16_t y)
{
    current_x = x;
    current_y = y;
}

void VLcd::draw_pixel(int16_t x, int16_t y, uint32_t color)
{
    if(x < 0 || x >= lcd.width)return;
    if(y < 0 || y >= lcd.width)return;

    uint32_t index = position_to_index(x, y);
    lcd.pixel[index].color.b = (color & 0xff0000) >> 16;
    lcd.pixel[index].color.g = (color & 0xff00) >> 8;
    lcd.pixel[index].color.r = (color & 0xff);
}
size_t VLcd::write(uint8_t ch)
{
    disp_char6x8(current_x, current_y, ch);
    current_x += 6;
    return 1;
}

void VLcd::fill_screen(uint32_t color)
{
    memset(lcd.pixel, 0, lcd.mem_size);

}
void VLcd::clear(uint32_t color)
{
    memset(lcd.pixel, 0, lcd.mem_size);
}

bool VLcd::is_pixel_vail(int16_t x, int16_t y)
{
    uint32_t index = position_to_index(x, y);
    if(lcd.pixel[index].color.g || lcd.pixel[index].color.b || lcd.pixel[index].color.r )
        return true;
    else
        return false;
}

void VLcd::print_1bit(Print &p)
{
    p.print("   ");
    for(int x = 0; x < lcd.width; x++)
    {
        p.printf("%3d", x);
    }
    p.println();
    for(int y = 0; y < lcd.height; y++)
    {
        p.printf("%2d", y);
        p.print(" ");
        for(int x = 0; x < lcd.width; x++)
        {
            if(is_pixel_vail(x, y) != 0)
                p.print("  *");
            else
                p.print("   ");
        }
        p.println();
    }
}

void VLcd::print_color(Print &p)
{
    p.println();
    for(int i = 0; i < lcd.width * lcd.height; i++)
    {
        if(i % (lcd.width) == 0)
            p.println();
        p.print('[');
        p.print(lcd.pixel[i].byte[0], HEX);
        p.print(lcd.pixel[i].byte[1], HEX);
        p.print(lcd.pixel[i].byte[2], HEX);
        p.print(']');
        p.print('\t');

    }
    p.println();
}

void VLcd::disp_char6x8(int16_t x, int16_t y, uint8_t ch)
{
    unsigned char i, j;
    unsigned short k;
    k = ch;
    if (k > 32) k -= 32;
    else k = 0;

    for(i = 0; i < 6; i++)
        for(j = 0; j < 8; j++)
        {
            if(font6x8[k][i] & (0x1 << j))
                draw_pixel(x + i, y + j, 1);
            else
            {
            }
        }
    x += 8;
}
void VLcd::disp_char8x16(int16_t x, int16_t y, uint8_t ch)
{
    unsigned char i, j;
    unsigned short k;
    k = ch;
    if (k > 32) k -= 32;
    else k = 0;

    for(i = 0; i < 16; i++)
        for(j = 0; j < 8; j++)
        {
            if(asc16[k * 16 + i] & (0x80 >> j))
                draw_pixel(x + j, y + i, 1);
            else
            {
            }
        }
    x += 8;
}

