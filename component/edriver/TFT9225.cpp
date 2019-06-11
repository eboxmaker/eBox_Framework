/**
  ******************************************************************************
  * @file    TFT9225.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2019/06/11
  * @brief   ili9225 lcd
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
#include "TFT9225.h"
#include "font.h"

#define TFTLCD_CMD  0	//写命令
#define TFTLCD_DATA 1	//写数据

void Lcd::begin()
{
  _width = X_MAX_PIXEL;
  _height = Y_MAX_PIXEL;
  _cs->mode(OUTPUT_PP);
  _rs->mode(OUTPUT_PP);
  _rst->mode(OUTPUT_PP);
  _wr->mode(OUTPUT_PP);
  _rd->mode(OUTPUT_PP);
  _db->mode(OUTPUT_PP_PU);

  _cs->set();
  _rs->set();
  _rst->set();
  _wr->set();
  _rd->set();
  _db->write(0xff);

  _init();
}

void Lcd::_init()
{
  _cs->set();
  _rs->set();
  _rst->reset();
  delay_ms(100);
  _rst->set();

  _write(0x00d0,0);
  _write(0x0003,1);

  _write(0x00eb,0);
  _write(0x0b00,1);

  _write(0x00ec,0);
  _write(0x000f,1);

  _write(0x00c7,0);
  _write(0x030f,1);

  _write(0x0001,0);
  _write(0x011c,1);

  _write(0x0002,0);
  _write(0x0100,1);

  _write(0x0003,0);
  _write(0x1030,1);

  _write(0x0007,0);
  _write(0x0000,1);

  _write(0x0008,0);
  _write(0x0808,1);

  _write(0x000f,0);
  _write(0x0901,1);

  _write(0x0010,0);
  _write(0x0000,1);
  //------------POWER CONTROL----------//

  _write(0x0011,0);
  _write(0x103b,1);	 //0x1b41

  _write(0x0012,0);
  _write(0x200e,1);

  _write(0x0013,0);
  _write(0x0052,1);

  _write(0x0014,0); //vcomh+vcoml
  _write(0x4658,1);//4b5c   4658

  _write(0x0030,0);
  _write(0x0000,1);

  _write(0x0031,0);
  _write(0x00db,1);

  _write(0x0032,0);
  _write(0x0000,1);

  _write(0x0033,0);
  _write(0x0000,1);

  _write(0x0034,0);
  _write(0x00db,1);

  _write(0x0035,0);
  _write(0x0000,1);

  _write(0x0036,0);
  _write(0x00af,1);


  _write(0x0037,0);
  _write(0x0000,1);

  _write(0x0038,0);
  _write(0x00db,1);

  _write(0x0039,0);
  _write(0x0000,1);

  _write(0x0020,0);
  _write(0x0000,1);

  _write(0x0021,0);
  _write(0x0000,1);

  //------------GAMMA CONTROL-----------//
  _write(0x0050,0);
  _write(0x0000,1);

  _write(0x0051,0);
  _write(0x0705,1);

  _write(0x0052,0);
  _write(0x0c0a,1);

  _write(0x0053,0);
  _write(0x0401,1);

  _write(0x0054,0);
  _write(0x040c,1);

  _write(0x0055,0);
  _write(0x0608,1);

  _write(0x0056,0);
  _write(0x0000,1);

  _write(0x0057,0);
  _write(0x0104,1);

  _write(0x0058,0);
  _write(0x0e06,1);

  _write(0x0059,0);
  _write(0x060e,1);


  delay_ms(20);

  _write(0x0007,0);
  _write(0x1017,1);
  _write(0x0022,0);  

}

void Lcd::_write(uint16_t dat,uint8_t cmd)
{
  _rs->write(cmd);
  _cs->reset();
  _db->write(dat>>8);
  _wr->reset();
  _wr->set();
  _db->write(dat);
  _wr->reset();
  _wr->set();
  _cs->set();
  _rs->set();
}
void Lcd::soft_reset()
{
//    write_index(0x01);//soft reset
}

void Lcd::on()
{
//    write_index(0x29);//Display on
}
void Lcd::off()
{
//    write_index(0x28);//Display off

}
void Lcd::column_order(uint8_t order)
{

}
void Lcd::row_order(uint8_t order)
{
//    write_index(0x36); //MX, MY, RGB mode
//    if(order == 1)
//    {
//        MADCTL |= 1 << 7;
//    }
//    else
//        MADCTL &= ~(1 << 7);
//    write_data_8bit(MADCTL);

}

/*************************************************
函数名：Lcd_Clear
功能：全屏清屏函数
入口参数：填充颜色COLOR
返回值：无
*************************************************/
void Lcd::clear(uint16_t Color)
{
  uint16_t i, j;
  _setRegion(0, 0);
  _write(0x0022,0);

  for (i=0; i<=176; i++)
  {
    for (j=0; j<=220; j++)
    {
      _write(Color,TFTLCD_DATA);
    }
  }
}
/*************************************************
函数名：LCD_Set_XY
功能：设置lcd显示起始点
入口参数：xy坐标
返回值：无
*************************************************/
void Lcd::set_xy(int16_t x, int16_t y)
{
  //set_region(x, y, x, y);
}



void Lcd::draw_pixel(int16_t x, int16_t y, uint32_t color)
{
  if (x < 0 || x > _width) return;
  if (y < 0 || y > _height) return;
  _setRegion(x,y);
  _write(color,TFTLCD_DATA);
}

void Lcd::fill_screen(uint32_t color)
{
  clear(color);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Lcd::h_disp_char8x16(int16_t x, int16_t y, uint8_t ch)
{
//    if(ch >= 0x20)ch -= 0x20;
//    set_region(x, y, x + 8, y + 16);
//    for (int i = 0; i <= 16; i++)
//    {
//        for (int j = 0; j <= 8; j++)
//        {
//            if(asc16[ch * 16 + i] & (0x80 >> j))
//                write_data_16bit(front_color);
//            else
//                write_data_16bit(back_color);
//        }
//    }
}
void Lcd::disp_char8x16(int16_t x, int16_t y, uint8_t ch)
{
//    unsigned char i, j;
//    unsigned short k;
//    k = ch;
//    if (k > 32) k -= 32;
//    else k = 0;

//    for(i = 0; i < 16; i++)
//        for(j = 0; j < 8; j++)
//        {
//            if(asc16[k * 16 + i] & (0x80 >> j))
//                draw_pixel(x + j, y + i, front_color);
//            else
//            {
//                if(text_mode == ENABLE_BACK_COLOR)draw_pixel(x + j, y + i, back_color);
//            }
//        }
//    x += 8;
}

void Lcd::printf(int16_t x, int16_t y, const char *fmt, ...)
{
  char buf[30];
  uint8_t i = 0;
  va_list va_params;
  va_start(va_params, fmt);
  ebox_vsprintf(buf, fmt, va_params);
  va_end(va_params);
  while (buf[i] != '\0')
  {
    if (x > 128)
    {
      y += 16;
      x = 0;
    }
    h_disp_char8x16(x, y, buf[i++]);
    x += 8;
  }
}
void Lcd::draw_font_gbk16(int16_t x, int16_t y, uint8_t *s)
{
//    unsigned char i, j;
//    unsigned short k, x0;
//    x0 = x;

//    while(*s)
//    {
//        if((*s) < 128)
//        {
//            k = *s;
//            if (k == 13)
//            {
//                x = x0;
//                y += 16;
//            }
//            else
//            {
//                if (k > 32) k -= 32;
//                else k = 0;

//                for(i = 0; i < 16; i++)
//                    for(j = 0; j < 8; j++)
//                    {
//                        if(asc16[k * 16 + i] & (0x80 >> j))	draw_pixel(x + j, y + i, front_color);
//                        else
//                        {
//                            if(text_mode == ENABLE_BACK_COLOR)draw_pixel(x + j, y + i, back_color);
//                        }
//                    }
//                x += 8;
//            }
//            s++;
//        }

//        else
//        {


//            for (k = 0; k < hz16_num; k++)
//            {
//                if ((hz16[k].Index[0] == *(s)) && (hz16[k].Index[1] == *(s + 1)))
//                {
//                    for(i = 0; i < 16; i++)
//                    {
//                        for(j = 0; j < 8; j++)
//                        {
//                            if(hz16[k].Msk[i * 2] & (0x80 >> j))	draw_pixel(x + j, y + i, front_color);
//                            else
//                            {
//                                if(text_mode == ENABLE_BACK_COLOR)draw_pixel(x + j, y + i, back_color);
//                            }
//                        }
//                        for(j = 0; j < 8; j++)
//                        {
//                            if(hz16[k].Msk[i * 2 + 1] & (0x80 >> j))	draw_pixel(x + j + 8, y + i, front_color);
//                            else
//                            {
//                                if(text_mode == ENABLE_BACK_COLOR)draw_pixel(x + j + 8, y + i, back_color);
//                            }
//                        }
//                    }
//                }
//            }
//            s += 2;
//            x += 16;
//        }

//    }
}
//取模方式 水平扫描 从左到右 低位在前
void Lcd::draw_bitmap(const unsigned char *p) //显示40*40 QQ图片
{
//    int i, j, k;
//    unsigned char picH, picL;
//    //clear(BLACK); //清屏

//    for(k = 0; k < 4; k++)
//    {
//        for(j = 0; j < 3; j++)
//        {
//            set_region(40 * j, 40 * k, 40 * j + 39, 40 * k + 39);		//坐标设置
//            for(i = 0; i < 40 * 40; i++)
//            {
//                picL = *(p + i * 2);	//数据低位在前
//                picH = *(p + i * 2 + 1);
//                write_data_16bit(picH << 8 | picL);
//            }
//        }
//    }
}

/*************************************************
功能：设置显示窗口和内存地址
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd::_setRegion(int16_t x_start, int16_t y_start)
{
  _write(0x0020,0);
  _write(y_start,TFTLCD_DATA);
  _write(x_start,TFTLCD_DATA);//_write((y<<8),1);
  _write(0x0021,0);
  _write(x_start,TFTLCD_DATA);
  _write(y_start,TFTLCD_DATA);//_write(x,0);
  _write(0x0022,0);
}
