/*******************************
1.8寸液晶屏幕驱动程序
驱动IC:ST7735S
接口：SPI（MODE2、3）
分辨率：128*160
色彩：256K
*******************************/
#include "lcd_1.8.h"
#include "font.h"

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

//LCD lcd(&PB13,&PB15,&PA13,&PA15,&PA14,&PA12);

void LCD::begin(u8 dev_num)
{
    cs->mode(OUTPUT_PP);
    led->mode(OUTPUT_PP);
    rs->mode(OUTPUT_PP);
    rst->mode(OUTPUT_PP);
    led->set();

    config.mode = SPI_MODE2;
    config.bit_order = MSB_FIRST;
    config.dev_num = dev_num;
    config.prescaler = SPI_CLOCK_DIV2;
    spi->begin(&config);

    init();
}
void LCD::soft_reset()
{
    write_index(0x01);//soft reset
}

//向SPI总线传输一个8位数据
//void  LCD::spi_write_data(u8 Data)
//{
//	unsigned char i=0;
//	for(i=8;i>0;i--)
//	{
//		if(Data&0x80)
//	  sda->set(); //输出数据
//      else sda->reset();
//
//      scl->reset();
//      scl->set();
//      Data<<=1;
//	}
//}
//向液晶屏写一个8位指令
void LCD::write_index(u8 Index)
{
    //SPI 写命令时序开始
    spi->take_spi_right(&config);
    cs->reset();
    rs->reset();
    spi->write(Index);
    cs->set();
    spi->release_spi_right();
}
//向液晶屏写一个8位数据
void LCD::write_data_8bit(u8 Data)
{
    spi->take_spi_right(&config);
    cs->reset();
    rs->set();
    spi->write(Data);
    cs->set();
    spi->release_spi_right();
}
//向液晶屏写一个16位数据
void LCD::write_data_16bit(u16 Data)
{
    spi->take_spi_right(&config);
    cs->reset();
    rs->set();
    spi->write(Data >> 8); 	//写入高8位数据
    spi->write(Data); 			//写入低8位数据
    cs->set();
    spi->release_spi_right();
}
//u8 LCD::read_8bit()
//{
//    u8 data;
//    cs->reset();
//    rs->set();
//    data = spi->read();
//    cs->set();
//    return data;
//}
//u16 LCD::read_16bit()
//{
//    u8 data;
//    cs->reset();
//    rs->set();
//    data |= spi->read();
//    data |= spi->read() << 8;
//    cs->set();
//    return data;
//}
void LCD::write_reg(u8 Index, u8 Data)
{
    write_index(Index);
    write_data_8bit(Data);
}
//u8 LCD::read_reg(u8 Index)
//{
//    u8 data;
//    write_index(Index);
//    data = read_8bit();
//    return data;
//}
void LCD::reset(void)
{
    rst->reset();
    delay_ms(1000);
    rst->set();
    delay_ms(50);
}
//LCD Init For 1.44Inch LCD Panel with ST7735R.
void LCD::init(void)
{
    reset(); //Reset before LCD Init.

    //LCD Init For 1.44Inch LCD Panel with ST7735R.
    write_index(0x11);//Sleep exit
    delay_ms (120);

    //ST7735R Frame Rate
    write_index(0xB1);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);

    write_index(0xB2);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);

    write_index(0xB3);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);
    write_data_8bit(0x01);
    write_data_8bit(0x2C);
    write_data_8bit(0x2D);

    write_index(0xB4); //Column inversion
    write_data_8bit(0x07);

    //ST7735R Power Sequence
    write_index(0xC0);
    write_data_8bit(0xA2);
    write_data_8bit(0x02);
    write_data_8bit(0x84);
    write_index(0xC1);
    write_data_8bit(0xC5);

    write_index(0xC2);
    write_data_8bit(0x0A);
    write_data_8bit(0x00);

    write_index(0xC3);
    write_data_8bit(0x8A);
    write_data_8bit(0x2A);
    write_index(0xC4);
    write_data_8bit(0x8A);
    write_data_8bit(0xEE);

    write_index(0xC5); //VCOM
    write_data_8bit(0x0E);

    MADCTL = 0XC0;
    write_index(0x36); //MX, MY, RGB mode
    write_data_8bit(MADCTL);

    //ST7735R Gamma Sequence
    write_index(0xe0);
    write_data_8bit(0x0f);
    write_data_8bit(0x1a);
    write_data_8bit(0x0f);
    write_data_8bit(0x18);
    write_data_8bit(0x2f);
    write_data_8bit(0x28);
    write_data_8bit(0x20);
    write_data_8bit(0x22);
    write_data_8bit(0x1f);
    write_data_8bit(0x1b);
    write_data_8bit(0x23);
    write_data_8bit(0x37);
    write_data_8bit(0x00);
    write_data_8bit(0x07);
    write_data_8bit(0x02);
    write_data_8bit(0x10);

    write_index(0xe1);
    write_data_8bit(0x0f);
    write_data_8bit(0x1b);
    write_data_8bit(0x0f);
    write_data_8bit(0x17);
    write_data_8bit(0x33);
    write_data_8bit(0x2c);
    write_data_8bit(0x29);
    write_data_8bit(0x2e);
    write_data_8bit(0x30);
    write_data_8bit(0x30);
    write_data_8bit(0x39);
    write_data_8bit(0x3f);
    write_data_8bit(0x00);
    write_data_8bit(0x07);
    write_data_8bit(0x03);
    write_data_8bit(0x10);

    write_index(0x2a);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x7f);

    write_index(0x2b);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x00);
    write_data_8bit(0x9f);

    write_index(0xF0); //Enable test command
    write_data_8bit(0x01);
    write_index(0xF6); //Disable ram power save mode
    write_data_8bit(0x00);

    write_index(0x3A); //65k mode
    write_data_8bit(0x05);

    ///////////////////////////
    write_index(0x0c);
    write_data_8bit(0x00);
    write_data_8bit(0x05);
    /////////////////////////////////
    write_index(0x29);//Display on
}

void LCD::on()
{
    write_index(0x29);//Display on
}
void LCD::off()
{
    write_index(0x28);//Display off

}
void LCD::column_order(u8 order)
{
    write_index(0x36); //MX, MY, RGB mode
    if(order == 1)
    {
        MADCTL |= 1 << 6;
    }
    else
        MADCTL &= ~(1 << 6);
    write_data_8bit(MADCTL);

}
void LCD::row_order(u8 order)
{
    write_index(0x36); //MX, MY, RGB mode
    if(order == 1)
    {
        MADCTL |= 1 << 7;
    }
    else
        MADCTL &= ~(1 << 7);
    write_data_8bit(MADCTL);

}
/*************************************************
函数名：set_region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void LCD::set_region(u16 x_start, u16 y_start, u16 x_end, u16 y_end)
{
    write_index(0x2a);
    write_data_8bit(0x00);
    write_data_8bit(x_start + 2);
    write_data_8bit(0x00);
    write_data_8bit(x_end + 2);

    write_index(0x2b);
    write_data_8bit(0x00);
    write_data_8bit(y_start + 1);
    write_data_8bit(0x00);
    write_data_8bit(y_end + 1);

    write_index(0x2c);

}
/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void LCD::clear(u16 Color)
{
    unsigned int i;
    set_region(0, 0, X_MAX_PIXEL - 1, Y_MAX_PIXEL - 1);
    write_index(0x2C);
    for(i = 0; i < X_MAX_PIXEL * Y_MAX_PIXEL; i++)
    {
        write_data_16bit(Color);
    }
}
/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void LCD::set_xy(u16 x, u16 y)
{
    set_region(x, y, x, y);
}


/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
inline void LCD::draw_point(u16 x, u16 y, u16 Data)
{
    set_region(x, y, x + 1, y + 1);
    write_data_16bit(Data);
}

/*****************************************
 函数功能：读TFT某一点的颜色
 出口参数：color  点颜色值
******************************************/
//u16 LCD::read_point(u16 x,u16 y)
//{
//  u16 data;
//  set_xy(x,y);

//    write_index(0x2e);

//    data = read_16bit();

//  return data;
//}



/*************************************************
函数名：draw_h_line
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void LCD::draw_h_line(int x0, int y,  int x1)
{
    set_region(x0, y, x1, y);
    for (; x0 <= x1; x0++)
    {
        write_data_16bit(front_color);
    }
}

/*************************************************
函数名：draw_v_line
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void LCD::draw_v_line(int x, int y0,  int y1)
{
    set_region(x, y0, x, y1);

    for (; y0 <= y1; y0++)
    {
        write_data_16bit(front_color);
    }
}
#include "color_convert.h"
void LCD::fill_rect(int x0, int y0,  int x1, int y1)
{
    u16 i = 0;
    u8 dx, dy;
    dx = x1 - x0 + 1;
    dy = y1 - y0 + 1;

    set_region(x0, y0, x1, y1);
    for (; i <= dx * dy; i++)
    {
        write_data_16bit(front_color);
    }
}
void LCD::fill_rect(int x0, int y0,  int x1, int y1, u16 *bitmap)
{
    u16 i = 0;
    u8 dx, dy;
    dx = x1 - x0 + 1;
    dy = y1 - y0 + 1;

    set_region(x0, y0, x1, y1);
    for (; i <= dx * dy; i++)
    {
        write_data_16bit(bitmap[i]);
    }

}
void LCD::draw_circle(u16 x, u16 y, u16 r)
{
    //Bresenham算法
    unsigned short  a, b;
    int c;
    a = 0;
    b = r;
    c = 3 - 2 * r;
    while (a < b)
    {
        draw_point(x + a, y + b, front_color); //        7
        draw_point(x - a, y + b, front_color); //        6
        draw_point(x + a, y - b, front_color); //        2
        draw_point(x - a, y - b, front_color); //        3
        draw_point(x + b, y + a, front_color); //        8
        draw_point(x - b, y + a, front_color); //        5
        draw_point(x + b, y - a, front_color); //        1
        draw_point(x - b, y - a, front_color); //        4

        if(c < 0) c = c + 4 * a + 6;
        else
        {
            c = c + 4 * (a - b) + 10;
            b -= 1;
        }
        a += 1;
    }
    if (a == b)
    {
        draw_point(x + a, y + b, front_color);
        draw_point(x + a, y + b, front_color);
        draw_point(x + a, y - b, front_color);
        draw_point(x - a, y - b, front_color);
        draw_point(x + b, y + a, front_color);
        draw_point(x - b, y + a, front_color);
        draw_point(x + b, y - a, front_color);
        draw_point(x - b, y - a, front_color);
    }

}
//画线函数，使用Bresenham 画线算法
void LCD::draw_line(u16 x0, u16 y0, u16 x1, u16 y1)
{
    int dx,             // difference in x's
        dy,             // difference in y's
        dx2,            // dx,dy * 2
        dy2,
        x_inc,          // amount in pixel space to move during drawing
        y_inc,          // amount in pixel space to move during drawing
        error,          // the discriminant i.e. error i.e. decision variable
        index;          // used for looping


    set_xy(x0, y0);
    dx = x1 - x0; //计算x距离
    dy = y1 - y0; //计算y距离

    if (dx >= 0)
    {
        x_inc = 1;
    }
    else
    {
        x_inc = -1;
        dx    = -dx;
    }

    if (dy >= 0)
    {
        y_inc = 1;
    }
    else
    {
        y_inc = -1;
        dy    = -dy;
    }

    dx2 = dx << 1;
    dy2 = dy << 1;

    if (dx > dy)//x距离大于y距离，那么每个x轴上只有一个点，每个y轴上有若干个点
    {
        //且线的点数等于x距离，以x轴递增画点
        // initialize error term
        error = dy2 - dx;

        // draw the line
        for (index = 0; index <= dx; index++) //要画的点数不会超过x距离
        {
            //画点
            draw_point(x0, y0, front_color);

            // test if error has overflowed
            if (error >= 0) //是否需要增加y坐标值
            {
                error -= dx2;

                // move to next line
                y0 += y_inc; //增加y坐标值
            } // end if error overflowed

            // adjust the error term
            error += dy2;

            // move to the next pixel
            x0 += x_inc; //x坐标值每次画点后都递增1
        } // end for
    } // end if |slope| <= 1
    else//y轴大于x轴，则每个y轴上只有一个点，x轴若干个点
    {
        //以y轴为递增画点
        // initialize error term
        error = dx2 - dy;

        // draw the line
        for (index = 0; index <= dy; index++)
        {
            // set the pixel
            draw_point(x0, y0, front_color);

            // test if error overflowed
            if (error >= 0)
            {
                error -= dy2;

                // move to next line
                x0 += x_inc;
            } // end if error overflowed

            // adjust the error term
            error += dx2;

            // move to the next pixel
            y0 += y_inc;
        } // end for
    } // end else |slope| > 1
}

void LCD::h_disp_char8x16(u16 x, u16 y, u8 ch)
{
    if(ch >= 0x20)ch -= 0x20;
    set_region(x, y, x + 8, y + 16);
    for (int i = 0; i <= 16; i++)
    {
        for (int j = 0; j <= 8; j++)
        {
            if(asc16[ch * 16 + i] & (0x80 >> j))
                write_data_16bit(front_color);
            else
                write_data_16bit(back_color);
        }
    }
}
void LCD::disp_char8x16(u16 x, u16 y, u8 ch)
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
                draw_point(x + j, y + i, front_color);
            else
            {
                if(text_mode == ENABLE_BACK_COLOR)draw_point(x + j, y + i, back_color);
            }
        }
    x += 8;
}
void LCD::printf(u16 x, u16 y, const char *fmt, ...)
{
    char buf[30];
    u8 i = 0;
    va_list va_params;
    va_start(va_params, fmt);
    vsprintf(buf, fmt, va_params);
    va_end(va_params);
    while(buf[i] != '\0')
    {
        if(x > 128)
        {
            y += 16;
            x = 0;
        }
        h_disp_char8x16(x, y, buf[i++]);
        x += 8;
    }
}
void LCD::draw_font_gbk16(u16 x, u16 y, u8 *s)
{
    unsigned char i, j;
    unsigned short k, x0;
    x0 = x;

    while(*s)
    {
        if((*s) < 128)
        {
            k = *s;
            if (k == 13)
            {
                x = x0;
                y += 16;
            }
            else
            {
                if (k > 32) k -= 32;
                else k = 0;

                for(i = 0; i < 16; i++)
                    for(j = 0; j < 8; j++)
                    {
                        if(asc16[k * 16 + i] & (0x80 >> j))	draw_point(x + j, y + i, front_color);
                        else
                        {
                            if(text_mode == ENABLE_BACK_COLOR)draw_point(x + j, y + i, back_color);
                        }
                    }
                x += 8;
            }
            s++;
        }

        else
        {


            for (k = 0; k < hz16_num; k++)
            {
                if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1)))
                {
                    for(i = 0; i < 16; i++)
                    {
                        for(j = 0; j < 8; j++)
                        {
                            if(hz16[k].Msk[i * 2] & (0x80 >> j))	draw_point(x + j, y + i, front_color);
                            else
                            {
                                if(text_mode == ENABLE_BACK_COLOR)draw_point(x + j, y + i, back_color);
                            }
                        }
                        for(j = 0; j < 8; j++)
                        {
                            if(hz16[k].Msk[i * 2 + 1] & (0x80 >> j))	draw_point(x + j + 8, y + i, front_color);
                            else
                            {
                                if(text_mode == ENABLE_BACK_COLOR)draw_point(x + j + 8, y + i, back_color);
                            }
                        }
                    }
                }
            }
            s += 2;
            x += 16;
        }

    }
}
//取模方式 水平扫描 从左到右 低位在前
void LCD::draw_bitmap(const unsigned char *p) //显示40*40 QQ图片
{
    int i, j, k;
    unsigned char picH, picL;
    clear(BLACK); //清屏

    for(k = 0; k < 4; k++)
    {
        for(j = 0; j < 3; j++)
        {
            set_region(40 * j, 40 * k, 40 * j + 39, 40 * k + 39);		//坐标设置
            for(i = 0; i < 40 * 40; i++)
            {
                picL = *(p + i * 2);	//数据低位在前
                picH = *(p + i * 2 + 1);
                write_data_16bit(picH << 8 | picL);
            }
        }
    }
}

