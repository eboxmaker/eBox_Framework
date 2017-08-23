/**
  ******************************************************************************
  * @file    lcd_1.8.cpp
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
/*******************************
1.8寸液晶屏幕驱动程序
驱动IC:ST7735S
接口：SPI（MODE2、3）
分辨率：128*160
色彩：256K
*******************************/
/******************************
    GND ->GND
    VCC -> 3.3V
    SCL -> PA5
    SDA -> PA7
    RES -> PB3
    DC  -> PB4
    CS  -> PB5
    BL  -> PB6
//本程序适用eBox
//              GND   电源地
//              VCC   接5V或3.3v电源
//              SCL   接PA5（SCL）
//              SDA   接PA7（SDA）
//              RES   接PB3 (RST)
//              DC    接PB4 (RS)
//              CS    接PB5 
//              BL    接PB6

*******************************/
#include "lcd_1.8.h"
#include "font.h"

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

//Lcd lcd(&PB13,&PB15,&PA13,&PA15,&PA14,&PA12);

void Lcd::begin(uint8_t dev_num)
{
    cs->mode(OUTPUT_PP);
    led->mode(OUTPUT_PP);
    rs->mode(OUTPUT_PP);
    rst->mode(OUTPUT_PP);
    led->set();

    config.mode = SPI_MODE2;
    config.bit_order = SPI_BITODER_MSB;
    config.dev_num = dev_num;
    config.prescaler = SPI_CLOCK_DIV2;
    spi->begin(&config);

    init();
}
void Lcd::soft_reset()
{
    write_index(0x01);//soft reset
}

void Lcd::on()
{
    write_index(0x29);//Display on
}
void Lcd::off()
{
    write_index(0x28);//Display off

}
void Lcd::column_order(uint8_t order)
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
void Lcd::row_order(uint8_t order)
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
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd::clear(uint16_t Color)
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
void Lcd::set_xy(uint16_t x, uint16_t y)
{
    set_region(x, y, x, y);
}



void Lcd::draw_pixel(u16 x, u16 y, u16 Data)
{
    set_region(x, y, x + 1, y + 1);
    write_data_16bit(Data);
}


/*************************************************
函数名：draw_h_line
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Lcd::draw_h_line(int x0, int y0, int x1, uint16_t color)
{
    set_region(x0, y0, x1, y0);
    for (; x0 <= x1; x0++)
    {
        write_data_16bit(color);
    }
}

/*************************************************
函数名：draw_v_line
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Lcd::draw_v_line(int x0, int y0,  int y1, uint16_t color)
{
    set_region(x0, y0, x0, y1);

    for (; y0 <= y1; y0++)
    {
        write_data_16bit(color);
    }
}
void Lcd::fill_rect(int x0, int y0,  int x1, int y1, uint16_t color)
{
    uint16_t i = 0;
    uint8_t dx, dy;
    dx = x1 - x0 + 1;
    dy = y1 - y0 + 1;

    set_region(x0, y0, x1, y1);
    for (; i <= dx * dy; i++)
    {
        write_data_16bit(color);
    }
}
void Lcd::fill_rect(int x0, int y0,  int x1, int y1, uint16_t *bitmap)
{
    uint16_t i = 0;
    uint8_t dx, dy;
    dx = x1 - x0 + 1;
    dy = y1 - y0 + 1;

    set_region(x0, y0, x1, y1);
    for (; i <= dx * dy; i++)
    {
        write_data_16bit(bitmap[i]);
    }

}

//画线函数，使用Bresenham 画线算法
void Lcd::draw_line(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
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
            draw_pixel(x0, y0, color);

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
            draw_pixel(x0, y0, color);

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
void Lcd::draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color)
{
    unsigned short  a, b;
    int c;
    a = 0;
    b = r;
    c = 3 - 2 * r;
    while (a < b)
    {
        draw_pixel(x + a, y + b, color); //        7
        draw_pixel(x - a, y + b, color); //        6
        draw_pixel(x + a, y - b, color); //        2
        draw_pixel(x - a, y - b, color); //        3
        draw_pixel(x + b, y + a, color); //        8
        draw_pixel(x - b, y + a, color); //        5
        draw_pixel(x + b, y - a, color); //        1
        draw_pixel(x - b, y - a, color); //        4

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
        draw_pixel(x + a, y + b, color);
        draw_pixel(x + a, y + b, color);
        draw_pixel(x + a, y - b, color);
        draw_pixel(x - a, y - b, color);
        draw_pixel(x + b, y + a, color);
        draw_pixel(x - b, y + a, color);
        draw_pixel(x + b, y - a, color);
        draw_pixel(x - b, y - a, color);
    }

}
//////////////////////////////////////////////////////////////////////////////////////////////////
void Lcd::h_disp_char8x16(uint16_t x, uint16_t y, uint8_t ch)
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
void Lcd::disp_char8x16(uint16_t x, uint16_t y, uint8_t ch)
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
void Lcd::printf(uint16_t x, uint16_t y, const char *fmt, ...)
{
    char buf[30];
    uint8_t i = 0;
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
void Lcd::draw_font_gbk16(uint16_t x, uint16_t y, uint8_t *s)
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
void Lcd::draw_bitmap(const unsigned char *p) //显示40*40 QQ图片
{
    int i, j, k;
    unsigned char picH, picL;
    //clear(BLACK); //清屏

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






//向液晶屏写一个8位指令
void Lcd::write_index(uint8_t Index)
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
void Lcd::write_data_8bit(uint8_t Data)
{
    spi->take_spi_right(&config);
    cs->reset();
    rs->set();
    spi->write(Data);
    cs->set();
    spi->release_spi_right();
}
//向液晶屏写一个16位数据
void Lcd::write_data_16bit(uint16_t Data)
{
    spi->take_spi_right(&config);
    cs->reset();
    rs->set();
    spi->write(Data >> 8); 	//写入高8位数据
    spi->write(Data); 			//写入低8位数据
    cs->set();
    spi->release_spi_right();
}

void Lcd::write_reg(uint8_t Index, uint8_t Data)
{
    write_index(Index);
    write_data_8bit(Data);
}
/*************************************************
函数名：set_region
功能：设置lcd显示区域，在此区域写点数据自动换行
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd::set_region(uint16_t x_start, uint16_t y_start, uint16_t x_end, uint16_t y_end)
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
void Lcd::reset(void)
{
    rst->reset();
    delay_ms(1000);
    rst->set();
    delay_ms(50);
}
//Lcd Init For 1.44Inch Lcd Panel with ST7735R.
void Lcd::init(void)
{
    reset(); //Reset before Lcd Init.

    //Lcd Init For 1.44Inch Lcd Panel with ST7735R.
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
/*************************************************
函数名：LCD_DrawPoint
功能：画一个点
入口参数：无
返回值：无
*************************************************/
void Lcd::draw_point(uint16_t x, uint16_t y, uint16_t Data)
{
    set_region(x, y, x + 1, y + 1);
    write_data_16bit(Data);
}
