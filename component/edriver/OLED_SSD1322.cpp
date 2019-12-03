/**
  ******************************************************************************
  * @file    oled_ssd1306.cpp
  * @author  shentq
  * @version V2.2
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

static uint8_t buffer[ SSD1322_LCDWIDTH * SSD1322_LCDHEIGHT / (8/BITS_PER_PIXEL) ] = {0};

/**********************************************************
 *                    函 数 声 明 区                      *
 **********************************************************/
OledSSD1322::OledSSD1322(Gpio *cs, Gpio *dc, Gpio *res, Gpio *scl_d0, Gpio *sda_d1)
{
    this->cs_pin	= cs;
    this->res_pin	= res;
    this->dc_pin	= dc;
    this->scl_pin	= scl_d0;
    this->sda_pin	= sda_d1;
    this->spi = NULL;
}
OledSSD1322::OledSSD1322(Gpio *cs, Gpio *dc,Gpio *res , Spi *spi)
{
    this->cs_pin	= cs;
    this->res_pin	= res;
    this->dc_pin	= dc;
    this->spi = spi;
}
void OledSSD1322::begin(void)	//初始化SSD1306
{
    buffer_size = SSD1322_LCDWIDTH * SSD1322_LCDHEIGHT / (8/BITS_PER_PIXEL);
    width = SSD1322_LCDWIDTH;
    hight = SSD1322_LCDHEIGHT;
    
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
        config.mode = Spi::MODE0;
        config.bit_order = Spi::MSB;
        config.prescaler = Spi::DIV8;
        spi->begin(&config);
    }
    for(uint32_t i = 0; i < (64*256/2); i++)
        buffer[i] = 0;
    init();
    flush();
}
void OledSSD1322::draw_pixel(int16_t x, int16_t y,uint32_t color)
{
    if(x < 0 || x >= width)return;
    if(y < 0 || y >= hight)return;
    
    color &= 0x0f;
#ifdef SSD1322_256_64_4 // 4 bits per pixel
	register uint8_t mask = ((x % 2) ? color : color << 4);
	register uint8_t *pBuf = &buffer[(x >> 1) + (y * (SSD1322_LCDWIDTH / 2))];
	register uint8_t b1 = *pBuf;
	b1 &= (x % 2) ? 0xF0 : 0x0F; // cleardown nibble to be replaced
	// write our value in
	*pBuf++ = b1 | mask;
#endif
    
 #ifdef SSD1322_256_64_1 // 1 bit per pixel
  register uint8_t *pBuf = &buffer[(x >> 3) + (y * (SSD1322_LCDWIDTH / 8))];
  switch (color)
  {
    case WHITE:	*pBuf |=  (0x80 >> (x%8)); break;
    case BLACK:	*pBuf &= ~(0x80 >> (x%8)); break;
    case INVERSE:	*pBuf ^=  (0x80 >> (x%8)); break;
  }
#endif
}
void OledSSD1322::fill_screen(uint32_t color)
{
    color &=0x0f;
   for(uint32_t i = 0; i < buffer_size; i++)
        buffer[i] = color | color << 4;
}
/***********************************************************
*   函数名称：
*   功能描述：   OLED清屏
*   参数列表：
*   返回结果：
*   备    注：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
***********************************************************/

void OledSSD1322::clear()
{
    fill_screen(0);
    flush();
}
/***********************************************************
*   函数名称：
*   功能描述：   OLED刷新
*   参数列表：
*   返回结果：
*   备    注：在做任何显示更新之后必须，将缓冲区内容填充至屏幕
***********************************************************/
void OledSSD1322::flush()
{
//  固定显示区域，不用重复设置
	set_column_addr(0,63);
	set_row_addr(0,63);

	write_cmd(0x5C);    //开始写RAM命令

	for(uint32_t i = 0; i < buffer_size; i++)
	{    
       write_data(buffer[i]);
	}
}


/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OledSSD1322::write_data_hard(uint8_t data)	//OLED写数据
{
    dc_pin->set();
    cs_pin->reset();
    spi->take(&config);
    spi->write(data);
    spi->release();
    cs_pin->set();
    dc_pin->set();

}
void OledSSD1322::write_cmd_hard(uint8_t cmd)	//OLED写命令  
{
    dc_pin->reset();
    cs_pin->reset();
    spi->take(&config);
    spi->write(cmd);
    spi->release();
    cs_pin->set();    
    dc_pin->set();
}


void OledSSD1322::write_data_soft(uint8_t dat)
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
void OledSSD1322::write_cmd_soft(uint8_t cmd)
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
void OledSSD1322::write_cmd(uint8_t cmd)
{
    if(spi != NULL)
    {
        write_cmd_hard(cmd);
    }else{
        write_cmd_soft(cmd);
    }
}
void OledSSD1322::write_data(uint8_t data)
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


void  OledSSD1322::set_column_addr(uint16_t x0, uint16_t x1)	// OLED 设置坐标
{
    write_cmd(0x15);			        // Set Column Address
	write_data(28+x0);				//   Default => 0x00
	write_data(28+x1);				//   Default => 0x77
}
void  OledSSD1322::set_row_addr(uint16_t y0, uint16_t y1)	// OLED 设置坐标
{
    write_cmd(0x75);			// Set Row Address
	write_data(y0);				//   Default => 0x00
	write_data(y1);				//   Default => 0x3F
}



/***********************************************************
*   函数名称：
*   功能描述：   OledSSD1322开启显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OledSSD1322::display_on(void)
{
    write_cmd(0x8d);	//SET DCDC命令
    write_cmd(0X14);  //DCDC ON
    write_cmd(0XAF);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   OledSSD1322关闭显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OledSSD1322::display_off(void)
{
    write_cmd(0x8d);	//SET DCDC命令
    write_cmd(0X10);  //DCDC ON
    write_cmd(0XAE);  //DISPLAY ON
}
/***********************************************************
*   函数名称：
*   功能描述：   OledSSD1322初始化
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OledSSD1322::init(void)
{
    res_pin->set();
    delay_ms(100);
    res_pin->reset();
    delay_ms(100);
    res_pin->set();

    
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
	write_data(0x14);	//14
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
    
//	write_cmd(0xA9); //   Disable Partial Display

	write_cmd(0xaf);	//Sleep Out
	//delay1(100);  	
    
    //设定显示区域
//    set_column_addr(0,63);
//	set_row_addr(0,63);
}

