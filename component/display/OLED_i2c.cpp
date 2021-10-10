#include "OLED_i2c.h"
#include "math.h"
bool OledI2c::begin(I2c::Speed_t speed)
{
    i2c->begin();
    i2c->setClock(speed);
    
    write_cmd(0xAE);
    write_cmd( 0x00); //---set low column address
    write_cmd( 0x10); //---set high column address
    write_cmd( 0x40); //--set start line address
    write_cmd( 0xB0); //--set page address
    write_cmd( 0x81); // contract control
    write_cmd( 0xFF); //--128
    write_cmd( 0xA1); //set segment remap
    write_cmd( 0xA6); //--normal / reverse
    write_cmd( 0xA8); //--set multiplex ratio(1 to 64)
    write_cmd( 0x3F); //--1/32 duty
    write_cmd( 0xC8); //Com scan direction
    write_cmd(0xD3); //-set display offset
    write_cmd( 0x00); //

    write_cmd( 0xD5); //set osc division
    write_cmd( 0x80); //

    write_cmd( 0xD8); //set area color mode off
    write_cmd( 0x05); //

    write_cmd(0xD9); //Set Pre-Charge Period
    write_cmd( 0xF1); //

    write_cmd( 0xDA); //set com pin configuartion
    write_cmd( 0x12); //

    write_cmd( 0xDB); //set Vcomh
    write_cmd( 0x30); //

    write_cmd( 0x8D); //set charge pump enable
    write_cmd( 0x14); //

    write_cmd( 0xAF); //--turn on oled panel
    return true;
}
void OledI2c::write_cmd(uint8_t cmd)
{
    i2c->beginTransmission(SlaveAddr);
    i2c->write(0);
    i2c->write(cmd);
    i2c->endTransmission();
}
void OledI2c::write_data(uint8_t data)
{
    i2c->beginTransmission(SlaveAddr);
    i2c->write(0x40);
    i2c->write(data);
    i2c->endTransmission();

}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
void OledI2c::clear(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {
        write_cmd(0xb0 + i); //设置页地址（0~7）
        write_cmd(0X00);//设置显示位置—列低地址
        write_cmd(0X10);//设置显示位置—列高地址
        for(n = 0; n < 128; n++)
            write_data(0X00);
    } //更新显示
}
void OledI2c::on(void)
{
    uint8_t i, n;
    for(i = 0; i < 8; i++)
    {

        write_cmd(0xb0 + i);
        write_cmd(0X00);
        write_cmd(0X10);
        for(n = 0; n < 128; n++)
            write_data(0xff);
        for(n = 0; n < 128; n++)
            write_data(0x01);
    } //更新显示
}

//开启OLED显示
void OledI2c::display_on(void)
{
    write_cmd(0X8D);
    write_cmd(0X14);
    write_cmd(0XAF);
}
//关闭OLED显示
void OledI2c::display_off(void)
{
    write_cmd(0X8D);
    write_cmd(0X10);
    write_cmd(0XAE);
}

void OledI2c::set_pos(unsigned char x, unsigned char y)
{
    write_cmd(0xb0 + y);
    write_cmd(((x & 0xf0) >> 4) | 0x10);
    write_cmd(x & 0x0f);
}
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示
//size:选择字体 16/12
void OledI2c::show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
{
    unsigned char c = 0, i = 0;
    c = chr - ' '; //得到偏移后的值
    if(x > Max_Column - 1)
    {
        x = 0;
        y = y + 2;
    }
    if(Char_Size == 16)
    {
        set_pos(x, y);
        for(i = 0; i < 8; i++)
            write_data(font8x16[c * 16 + i]);

        set_pos(x, y + 1);
        for(i = 0; i < 8; i++)
            write_data(font8x16[c * 16 + i + 8]);
    }
    else
    {
        set_pos(x, y);
        for(i = 0; i < 6; i++)
            write_data(font6x8[c][i]);
    }
}
//显示一个字符号串
void OledI2c::show_string(uint8_t x, uint8_t y, char *chr, uint8_t Char_Size)
{
    unsigned char j = 0;
    while (chr[j] != '\0')
    {
        show_char(x, y, chr[j], Char_Size);
        x += 8;
        if(x > 120)
        {
            x = 0;
            y += 2;
        }
        j++;
    }
}

//显示2个数字
//x,y :起点坐标
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);
void OledI2c::show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (int)(num / pow((float)10, (float)len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                show_char(x + (size2 / 2)*t, y, ' ', size2);
                continue;
            }
            else enshow = 1;

        }
        show_char(x + (size2 / 2)*t, y, temp + '0', size2);
    }
}

//显示汉字
void OledI2c::show_chinese(uint8_t x, uint8_t y, uint8_t no)
{
    uint8_t t, adder = 0;
    set_pos(x, y);
    for(t = 0; t < 16; t++)
    {
        write_data(Hzk[2 * no][t]);
        adder += 1;
    }
    set_pos(x, y + 1);
    for(t = 0; t < 16; t++)
    {
        write_data(Hzk[2 * no + 1][t]);
        adder += 1;
    }
}

/********************************************
// fill_Picture
********************************************/
void OledI2c::fill_picture(unsigned char fill_Data)
{
    unsigned char m, n;
    for(m = 0; m < 8; m++)
    {
        write_cmd(0xb0 + m); //设置页地址（0~7）
        write_cmd(0X00);//设置显示位置—列低地址
        write_cmd(0X10);//设置显示位置—列高地址

        for(n = 0; n < 128; n++)
        {
            write_data(fill_Data);
        }
    }
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OledI2c::draw_bmp(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
    unsigned int j = 0;
    unsigned char x, y;

    if(y1 % 8 == 0) y = y1 / 8;
    else y = y1 / 8 + 1;
    for(y = y0; y < y1; y++)
    {
        set_pos(x0, y);
        for(x = x0; x < x1; x++)
        {
            write_data(BMP[j++]);
        }
    }
}



