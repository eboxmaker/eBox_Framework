/**
  ******************************************************************************
  * @file    TFT9325.cpp
  * @author  cat_li
  * @version V1.0
  * @date    2019/06/11
  * @brief   ili9225 lcd 驱动，可修改后支持9325
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

typedef struct
{
  uint8_t reg;
  uint16_t cfg;
}tft_reg_info;

#if (ID == 9225)
	#define LCD_REG_03_EnMode				 0x03
	#define LCD_REG_22_MWrte				 0x22
	#define LCD_REG_20_GHAdd				 0x20
	#define LCD_REG_21_GVAdd				 0x21
	#define LCD_REG_HSAdd            0x37		// Horizontal Start Address Set
	#define LCD_REG_HEAdd            0x36		// Horizontal End Address Set
	#define LCD_REG_VSAdd            0x39		// Vertical Start Address Set
	#define LCD_REG_VEAdd            0x38		// Vertical End Address Set

	#define AM0					0<<3		// GRAM水平更新
	#define AM1					1<<3		// GRAM垂直更新

	#define ID0_0				0				// 水平减少
	#define ID0_1				1<<4		// 水平增加
	#define ID1_0				0				// 垂直减少
	#define ID1_1				1<<5		// 垂直增加
	#define	RGB					0
	#define	BGR					1<<12

	// 显示方向
	#define SS0					0				// 正序S1-S720
	#define SS1					1<<8		// 倒序S720-S1
	#define	GS0					0				// G1-G320
	#define GS1					1<<9		// G320-G1
	// 扫描方式
	#define SM0					0				// 逐行
	#define SM1					1<<10		// 隔行		

	#if (degree == 90)//
		//R01H:SM=0,GS=0,SS=0 (for details,See the datasheet of ILI9225)	
		#define REG01_CFG 0x001C|SM0|SS0|GS0 // set the display line number and display direction
		//R03H:BGR=1,ID0=1,ID1=1,AM=1 (for details,See the datasheet of ILI9225)
		#define REG03_H				BGR|ID0_1|ID1_1|AM1
		#define REG03_V				BGR|ID0_1|ID1_1|AM0
	#else
		#define REG01_CFG 0x001C|SM0|SS1|GS0
		#define REG03_H				BGR|ID0_1|ID1_1|AM0
		#define REG03_V				BGR|ID0_1|ID1_1|AM1
	#endif
		#define REG07_CFG			0x1017		// 开显示，8色
	static tft_reg_info ili9225 [] = {
		/* init tft reg  */
		{0,0},
		{0x10, 0x0000 }, // Set SAP,DSTB,STB
		{0x11, 0x0000 }, // Set APON,PON,AON,VCI1EN,VC
		{0x12, 0x0000 }, // Set BT,DC1,DC2,DC3
		{0x13, 0x0000 }, // Set GVDD
		{0x14, 0x0000 }, // Set VCOMH/VCOML voltage

		// Please follow this power on sequence
		{0x11, 0x0018 }, // Set APON,PON,AON,VCI1EN,VC
		{0x12, 0x1121 }, // Set BT,DC1,DC2,DC3
		{0x13, 0x0063 }, // Set GVDD
		{0x14, 0x3961 }, // Set VCOMH/VCOML voltage
		{0x10, 0x0800 }, // Set SAP,DSTB,STB
		{0x11, 0x1038 }, // Set APON,PON,AON,VCI1EN,VC

		{0x02, 0x0100 }, // set 1 line inversion
		{0x01, REG01_CFG}, // set the display line number and display direction
		{0x03, REG03_H }, // set GRAM write direction .
		
		{0x07, 0x0000 }, // Display off
		{0x08, 0x0808 },
		{0x0B, 0x1100 },
		{0x0C, 0x0000 },
		{0x0F, 0x0501 }, // Set Osc
		{0x15, 0x0020 }, // Set VCI recycling
		{0x20, 0x0000 }, // RAM Address
		{0x21, 0x0000 }, // RAM Address

		//------------------------ Set GRAM area --------------------------------//
		{0x30, 0x0000 },
		{0x31, 0x00DB },
		{0x32, 0x0000 },
		{0x33, 0x0000 },
		{0x34, 0x00DB },
		{0x35, 0x0000 },
		{0x36, 0x00AF },
		{0x37, 0x0000 },
		{0x38, 0x00DB },
		{0x39, 0x0000 },
		// ---------- Adjust the Gamma 2.2 Curve -------------------//
		{0x50, 0x0603},
		{0x51, 0x080D},
		{0x52, 0x0D0C},
		{0x53, 0x0205},
		{0x54, 0x040A},
		{0x55, 0x0703},
		{0x56, 0x0300},
		{0x57, 0x0400},
		{0x58, 0x0B00},
		{0x59, 0x0017},

		{0x07, REG07_CFG}, // Vertical RAM Address Position
	};
#endif


	/*
		176x220屏幕物理坐标(px,py)如下: 当SM=0

SS1-->									 <--SS0
GS0|	  -------------------          -------------------
	 |	 |(0,0)              |        |(0,0)              |
	\/	 |                   |        |
			 |  ^           ^    |        |   ^           ^   |
		   |  |           |    |        |   |           |   |
		   |  |           |    |        |   |           |   |
		   |  |           |    |        |   |           |   |
		   |  |  ------>  |    |        |   | <------   |   |
		   |  |           |    |        |   |           |   |
		   |  |           |    |        |   |           |   |
		   |  |           |    |        |   |           |   |
		   |  |           |    |        |   |           |   |
		   |                   |        |
		   |                   |        |                   |
		/\ |      (x=176,y=220)|        |      (x=239,y=399)|
		|  |-------------------|        |-------------------|
GS1	|  |                   |        |                   |
		    -------------------          -------------------
				
*/

void Lcd::begin()
{
#if (degree == 90)
  _width = Y_MAX_PIXEL;
  _height = X_MAX_PIXEL;
#else
  _width = X_MAX_PIXEL;
  _height = Y_MAX_PIXEL;
#endif

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

	for(uint8_t i=0;i<sizeof(ili9225)/sizeof(ili9225[0]);i++)
	{
	_writeCommand(ili9225[i].reg,ili9225[i].cfg);
	}
}

void Lcd::on()
{
	_writeCommand(0x07, REG07_CFG);
}
void Lcd::off()
{
  _writeCommand(0x07, 0x0); 
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
  set_xy(0, 0);
  _setIndexReg(LCD_REG_22_MWrte);

  for (i=0; i<=_width ; i++)
  {
    for (j=0; j<=_height; j++)
    {
			_writeDate(Color);
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
#if (degree == 90)
	_writeCommand(LCD_REG_20_GHAdd, y);
  _writeCommand(LCD_REG_21_GVAdd, x); 
#else
	_writeCommand(LCD_REG_20_GHAdd, x);
  _writeCommand(LCD_REG_21_GVAdd, y); 
#endif

}



void Lcd::draw_pixel(int16_t x, int16_t y, uint32_t color)
{
  if (x < 0 || x > _width) return;
  if (y < 0 || y > _height) return;
  set_xy(x,y);
  /* Prepare to write GRAM */
  _setIndexReg(LCD_REG_22_MWrte);
  /* Write 16-bit GRAM Reg */
  _writeDate(color);
}

void  Lcd::fill_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t color){
	_setRegion(x,y,(x1-x),(y1-y));
	uint32_t size = (x1-x+1)*(y1-y+1);
	_setIndexReg(LCD_REG_22_MWrte);
	for(uint32_t i=0;i<size;i++)
	{		_writeDate(color);
	}
}

void  Lcd::fill_rect(int16_t x, int16_t y, int16_t x1, int16_t y1, uint32_t *color,uint16_t size){
	_setRegion(x,y,(x1-x),(y1-y));
	_setIndexReg(LCD_REG_22_MWrte);
	for(uint32_t i=0;i<size;i++)
	{		_writeDate(*color++);
	}
}

void Lcd::fill_screen(uint32_t color)
{
  clear(color);
}

void Lcd::draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint32_t color){
	  uint16_t counter = 0;
  _writeCommand(LCD_REG_03_EnMode, REG03_V);
  
  /* Set Cursor */
  set_xy(x0, y0);
  
  /* Prepare to write GRAM */
  _setIndexReg(LCD_REG_22_MWrte);

  /* Fill a complete vertical line */
  for(counter = y0; counter < y1; counter++)
  {
    _writeDate(color);
  }
  _writeCommand(LCD_REG_03_EnMode, REG03_H);  
}

void Lcd::draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint32_t color){
	  
  set_xy(x0, y0);  
  /* Prepare to write GRAM */
  _setIndexReg(LCD_REG_22_MWrte);
  /* Fill a complete vertical line */
  for(uint16_t counter = x0; counter < x1; counter++)
  {	
    _writeDate(color);
  }
}

/*************************************************
功能：设置显示窗口和内存地址
入口参数：xy起点和终点
返回值：无
*************************************************/
void Lcd::_setRegion(int16_t Xpos, int16_t Ypos,int16_t Width, int16_t Height)
{
#if (degree == 90)
  _writeCommand(LCD_REG_HSAdd, Ypos);
  /* Horizontal GRAM End Address */
  _writeCommand(LCD_REG_HEAdd, Ypos+Height);  
  /* Vertical GRAM Start Address */
  _writeCommand(LCD_REG_VSAdd, Xpos);
  /* Vertical GRAM End Address */
  _writeCommand(LCD_REG_VEAdd, (Xpos+Width)); 
#else
	  _writeCommand(LCD_REG_HSAdd, (Xpos));
  /* Horizontal GRAM End Address */
  _writeCommand(LCD_REG_HEAdd, (Xpos+Width));  
  /* Vertical GRAM Start Address */
  _writeCommand(LCD_REG_VSAdd, Ypos);
  /* Vertical GRAM End Address */
  _writeCommand(LCD_REG_VEAdd, Ypos+Height); 
#endif	
}

void Lcd::_setCommandMode()
{
		  _cs->set();
			_rs->reset();
			_cs->reset();
}

void Lcd::_setDataMode()
{
			_cs->set();
			_rs->set();
			_cs->reset();
}

void Lcd::_setWrite()
{
		_wr->reset();
		_wr->set();
}

void Lcd::_writeCommand(uint16_t cmd,uint16_t dat)
{
		_setCommandMode();
		_db->write(cmd>>8);
		_setWrite();
		_db->write(cmd);
		_setWrite();
	
		_setDataMode();
		_db->write(dat>>8);
		_setWrite();
		_db->write(dat);
		_setWrite();	
}

void Lcd::_setIndexReg(uint16_t reg)
{
	_setCommandMode();
	_db->write(reg>>8);
	_setWrite();
	_db->write(reg);
	_setWrite();
}

void Lcd::_writeDate(uint16_t reg)
{
	_setDataMode();
	_db->write(reg>>8);
	_setWrite();
	_db->write(reg);
	_setWrite();
}

// 有bug
uint16_t Lcd::_readReg(uint16_t reg)
{
	uint16_t res;
  
  _setIndexReg(reg);
	_setDataMode();
  
	_rd->set();
  _wr->reset();
//  _rd->reset();
//  _rd->set();
	_wr->set();
	res = _db->read()<<8; 
//	_rd->reset();

	_wr->reset();
	_wr->set();
//  _rd->set();
	res |= _db->read(); 
	_rd->reset();
  _rd->set();	
}
