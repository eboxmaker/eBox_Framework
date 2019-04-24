/**
  ******************************************************************************
  * @file    oled_ssd1306.cpp
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
#include "oled_ssd1322.h"
#include "font.h"


/**********************************************************
 *                    函 数 声 明 区                      *
 **********************************************************/
Oled4Spi::Oled4Spi(Gpio *cs, Gpio *dc, Gpio *res, Gpio *scl_d0, Gpio *sda_d1)
{
    this->cs_pin	= cs;
    this->res_pin	= res;
    this->dc_pin	= dc;
    this->scl_pin	= scl_d0;
    this->sda_pin	= sda_d1;
    this->spi = NULL;
}
Oled4Spi::Oled4Spi(Gpio *cs, Gpio *dc,Gpio *res , Spi *spi)
{
    this->cs_pin	= cs;
    this->res_pin	= res;
    this->dc_pin	= dc;
    this->spi = spi;
}
void Oled4Spi::begin(void)	//初始化SSD1306
{
    res_pin->mode(OUTPUT_PP);
    dc_pin->mode(OUTPUT_PP);
    cs_pin->mode(OUTPUT_PP);
    if(spi == NULL)
    {
        scl_pin->mode(OUTPUT_PP);
        sda_pin->mode(OUTPUT_PP);
    }
    else
    {
        config.dev_num = cs_pin->id;
        config.mode = SPI_MODE0;
        config.bit_order = MSB_FIRST;
        config.prescaler = SPI_CLOCK_DIV8;
        spi->begin(&config);
    }
    init();
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::write_data_hard(uint8_t data)	//OLED写数据
{
    dc_pin->set();
    cs_pin->reset();
    spi->take(&config);
    spi->write(data);
    spi->release();
    cs_pin->set();
    dc_pin->set();

}
void Oled4Spi::write_cmd_hard(uint8_t cmd)	//OLED写命令  
{
    dc_pin->reset();
    cs_pin->reset();
    spi->take(&config);
    spi->write(cmd);
    spi->release();
    cs_pin->set();    
    dc_pin->set();
}


void Oled4Spi::write_data_soft(uint8_t dat)
{

        uint8_t i;
        dc_pin->set();
        cs_pin->reset();
        for(i = 0; i < 8; i++)
        {
            scl_pin->reset();
            if(dat & 0x80)
                sda_pin->set();
            else
                sda_pin->reset();
            scl_pin->set();
            dat <<= 1;
        }
        cs_pin->set();
        dc_pin->set();
   

}

/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::write_cmd_soft(uint8_t cmd)
{

        uint8_t i;
        dc_pin->reset();
        cs_pin->reset();
        for(i = 0; i < 8; i++)
        {
            scl_pin->reset();
            if(cmd & 0x80)
                sda_pin->set();
            else
                sda_pin->reset();
            scl_pin->set();
            cmd <<= 1;
        }
        cs_pin->set();
        dc_pin->set();
    
}
void Oled4Spi::write_cmd(uint8_t cmd)
{
    if(spi != NULL)
    {
        write_cmd_hard(cmd);
    }else{
        write_cmd_soft(cmd);
    }
}
void Oled4Spi::write_data(uint8_t data)
{
    if(spi != NULL)
    {
        write_data_hard(data);
    }else{
        write_data_soft(data);
    }
}
/***********************************************************
*   函数名称：
*   功能描述：   OLED设置坐标
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::
set_pos(uint16_t x, uint16_t y)
{
    write_cmd(0xb0 + y);
    write_cmd(((x & 0xf0) >> 4) | 0x10);
    write_cmd((x & 0x0f) | 0x01);
}
void Oled4Spi::draw_point(uint8_t x, uint8_t y)
{
    uint8_t temp = 1 << (y % 8);
    set_pos(x, y / 8);

    write_data(temp);

}


void  Oled4Spi::set_column_addr(uint16_t x0, uint16_t x1)	// OLED 设置坐标
{
    write_cmd(0x15);			        // Set Column Address
	write_data(28+x0);				//   Default => 0x00
	write_data(28+x1);				//   Default => 0x77
}
void  Oled4Spi::set_row_addr(uint16_t y0, uint16_t y1)	// OLED 设置坐标
{
    write_cmd(0x75);			// Set Row Address
	write_data(y0);				//   Default => 0x00
	write_data(y1);				//   Default => 0x3F
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED清屏
*   参数列表：
*   返回结果：
*   备    注：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
***********************************************************/

void Oled4Spi::clear()
{
//    uint32_t i, n;
//    set_x_addr(0,127);
//    set_y_addr(0,63);
//    
//    write_cmd(0x5C);    //开始写RAM命令

//    for(i = 0; i < 64*128; i++)
//    {
//        write_data(value);
//    } //更新显示
    int  x,y;

	set_column_addr(0,63);
	set_row_addr(0,63);
        
	write_cmd(0x5C);    //开始写RAM命令
	
	for(y=0;y<64*128;y++)
	{                   
			write_data(0);
	}
}
/***********************************************************
*   函数名称：
*   功能描述：   Oled4Spi开启显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::display_on(void)
{
    write_cmd(0x8d);	//SET DCDC命令
    write_cmd(0X14);  //DCDC ON
    write_cmd(0XAF);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   Oled4Spi关闭显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::display_off(void)
{
    write_cmd(0x8d);	//SET DCDC命令
    write_cmd(0X10);  //DCDC ON
    write_cmd(0XAE);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示单个字符
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个字符,包括部分字符
***********************************************************/
void Oled4Spi::show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size)
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
        set_pos(x, y + 1);
        for(i = 0; i < 6; i++)
            write_data(font6x8[c][i]);
    }
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个字符串
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个字符串
***********************************************************/
void Oled4Spi::show_string(uint8_t x, uint8_t y, char *chr, uint8_t Char_Size)
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

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个字符串
*   参数列表：len :数字的位数，size:字体大小，mode:模式	0,填充模式;1,叠加模式
*			  num:数值(0~4294967295);
*   返回结果：
*   备    注：在指定位置显示一个字符串
***********************************************************/
void Oled4Spi::show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size)
{
    uint8_t t, temp;
    uint8_t enshow = 0;
    for(t = 0; t < len; t++)
    {
        temp = (num / oled_pow(10, len - t - 1)) % 10;
        if(enshow == 0 && t < (len - 1))
        {
            if(temp == 0)
            {
                show_char(x + (size / 2)*t, y, ' ', size);
                continue;
            }
            else enshow = 1;

        }
        show_char(x + (size / 2)*t, y, temp + '0', size);
    }
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个汉字
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个汉字
***********************************************************/
void Oled4Spi::show_chinese(uint8_t x, uint8_t y, uint8_t no)
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


/***********************************************************
*   函数名称：
*   功能描述：显示BMP图片128×64起始点坐标(x,y),
*   参数列表：x的范围0～127，y为页的范围0～7
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::draw_bmp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const unsigned char BMP[])
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

/*****************  m^n  ********************/
uint32_t Oled4Spi::oled_pow(uint8_t m, uint8_t n)
{
    uint32_t result = 1;
    while(n--)result *= m;
    return result;
}


void Oled4Spi::draw_bmp(uint8_t *BMP,uint8_t Gray_Level,uint8_t Color)//BMP为图像数据，Gray_Level=0时全屏只显示一种颜色，Gray_Level=1时显示支持灰度图片；Color为灰度值0～15;
{/*****************16x16点阵的上半部分，即上8x16****************************************/
	uint8_t i,j,k;
	uint8_t Data1,Data2,Data3,Data4,DATA1=0,DATA2=0;
	if(Gray_Level==0){//单一颜色图片，灰度值为Color
		for(k=0;k<8;k++)
	{
        
	set_column_addr(0,64);	
	set_row_addr(8*k,8*k+7);
	write_cmd(0x5c);
						
						{
						for(j=0;j<(64);j++)
						{
							Data1=*BMP++;
							Data2=*BMP++;
							Data3=*BMP++;
							Data4=*BMP++;
							for(i=0;i<8;i++)
							{
								if(Data1&(0x01<<i))	
								{
									DATA1=(Color << 4);
								}
								if(Data2&(0x01<<i))
								{
									DATA1|=Color;
								}	
								write_data (DATA1);
								DATA1=0;
								if(Data3&(0x01<<i))	
								{
									DATA2=(Color << 4);
								}
								if(Data4&(0x01<<i))
								{
									DATA2|=Color;
								}
								
								write_data (DATA2);
								DATA2=0;
							}
						}
					}

					}			
		}				
			else{//16位灰度图片（取模方向为数据垂直，字节水平）
		for(k=0;k<64;k++)
	{
	set_column_addr(k,k+2);	
	set_row_addr(0,63);
	write_cmd(0x5c);
						
						{
						for(j=0;j<(64);j++)
						{
		
								
								write_data (*(BMP+128*k+j));
							write_data (*(BMP+128*k+j+64));									
						}
	
		}

	}
}

}


/***********************************************************
*   函数名称：
*   功能描述：   Oled4Spi初始化
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void Oled4Spi::init(void)
{
    res_pin->set();
    delay_ms(100);
    res_pin->reset();
    delay_ms(100);
    res_pin->set();

 /*
    write_cmd(0xae);//--turn off oled panel
    write_cmd(0x00);//---set low column address
    write_cmd(0x10);//---set high column address
    write_cmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
    write_cmd(0x81);//--set contrast control register
    write_cmd(0xcf); // Set SEG Output Current Brightness
    write_cmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
    write_cmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
    write_cmd(0xa6);//--set normal display
    write_cmd(0xa8);//--set multiplex ratio(1 to 64)
    write_cmd(0x3f);//--1/64 duty
    write_cmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
    write_cmd(0x00);//-not offset
    write_cmd(0xd5);//--set display clock divide ratio/oscillator frequency
    write_cmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
    write_cmd(0xd9);//--set pre-charge period
    write_cmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
    write_cmd(0xda);//--set com pins hardware configuration
    write_cmd(0x12);
    write_cmd(0xdb);//--set vcomh
    write_cmd(0x40);//Set VCOM Deselect Level
    write_cmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
    write_cmd(0x02);//
    write_cmd(0x8d);//--set Charge Pump enable/disable
    write_cmd(0x14);//--set(0x10) disable
    write_cmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
    write_cmd(0xa6);// Disable Inverse Display On (0xa6/a7)
    write_cmd(0xaf);//--turn on oled panel

    write_cmd(0xaf);	/display ON/
    clear();
    set_pos(0, 0);
    */
    
 	write_cmd(0xae); //AE:display OFF  AF: ON 
   
    
    write_cmd(0xfd);  /*Command Lock*/ 
	write_data(0x12); //0x12:解锁    0x16:Lock
	
	
	write_cmd(0xb3);	//Set Display Clock Divide Ratio/Oscillator Frequency 
	write_data(0x91);   //主频分频器
	
	write_cmd(0xca);	//Set Multiplex Ratio 
	write_data(0x3f); //使用64根行线
	
	write_cmd(0xa2);	//Set Display Offset 
	write_data(0x00);	//
	
	write_cmd(0xa1);	//Set Display Start Line 
	write_data(0x00);	//
	
	write_cmd(0xa0);	//Set Re-Map $ Dual COM Line Mode
	write_data(0x10);	//14
//	write_data(0x01);	//0x11
	
	write_cmd(0xB5);	//Set GPIO 
	write_data(0x00);
		
	write_cmd(0xab);	//Function Selection
	write_data(0x01);	//
	
	write_cmd(0xb4);	//Enable External VSL 
	write_data(0xa0);	//
	write_data(0xfd);	//f8
	
	write_cmd(0xc1);	//Set Contrast Current
	write_data(0xff);	//ef
	
	write_cmd(0xc7);	//Master Contrast Current Control
	write_data(0x0f);	//ff
	/*
	write_cmd(0xb8);	//Select Gray Scale Table
	write_data(0x00);	
	write_data(0x01);	
	write_data(0x03);	
	write_data(0x06);	
	write_data(0x0a);
	write_data(0x10);	
	write_data(0x1a);	
	write_data(0x28);	
	write_data(0x37);	
	write_data(0x47);	
	write_data(0x58);	
	write_data(0x6a);	
	write_data(0x7f);	
	write_data(0x96);	
	write_data(0xb4);
	*/
	write_cmd(0xb9);	//Select Default Linear Gray Scale Table 
	
	write_cmd(0xb1);	//Set Phase Length
	write_data(0xe2);	
	
	write_cmd(0xd1);	//Enhance Driving Scheme Capability 
	write_data(0xa2);	//a2
	write_data(0x20);	
	
	write_cmd(0xbb);	//Set Pre-Charge Voltage 
	write_data(0x1f);	

	write_cmd(0xb6);	//Set Second Pre-Charge Period 
	write_data(0x08);	
	
	write_cmd(0xbe);	//Set VCOMH Deselect Level 
	write_data(0x07);		
	
	write_cmd(0xa6);	//Set Display Mode
	//Clear Screen
    
//	write_cmd(0xA9); //   Disable Partial Display

	write_cmd(0xaf);	//Sleep Out
	//delay1(100);  	
}
#include "ebox.h"
void Oled4Spi::clear(uint8_t value)
{
//    uint32_t i, n;
//    set_x_addr(0,127);
//    set_y_addr(0,63);
//    
//    write_cmd(0x5C);    //开始写RAM命令

//    for(i = 0; i < 64*128; i++)
//    {
//        write_data(value);
//    } //更新显示
    int  x,y;

	set_column_addr(0,63);
	set_row_addr(0,63);

	write_cmd(0x5C);    //开始写RAM命令

	for(y=0;y<64;y++)
	{    
        for(x=0; x <128;x++)
        {
                write_data(value | (value << 4));
        }
        
	}
}
void Oled4Spi::fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot)
{
    set_column_addr(0,63);
	set_row_addr(0,63);
    
}
