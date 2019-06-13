/**
  ******************************************************************************
  * @file    TFT9325.cpp
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
#include "TFT9325.h"
#include "font.h"

#define TFTLCD_CMD  0	//写命令
#define TFTLCD_DATA 1	//写数据

/** 
  * @brief  ILI9325 Registers  
  */ 
#define LCD_REG_00_ID             0x00   // ID code
#define LCD_REG_01_DOC1           0x01   // Driver Output Control
#define LCD_REG_02_lcdDC          0x02   // LCD Driving Control
#define LCD_REG_03_EnMode         0x03   // Entry Mode
#define LCD_REG_04_Resize         0x04   // Resize Control
#define LCD_REG_05             0x05
#define LCD_REG_06             0x06
#define LCD_REG_07_DC1            0x07   // Display Control 1
#define LCD_REG_08_DC2            0x08   // Display Control 2
#define LCD_REG_09_DC3            0x09   // Display Control 3
#define LCD_REG_0A_DC4            0x0A   // Display Control 4
#define LCD_REG_0C_RGBDIC1        0x0C   // RGB Display Interface Control 1
#define LCD_REG_0D_Frame          0x0D   // Frame Maker Position 
#define LCD_REG_0F_RGBDIC2        0x0F   // RGB Display Interface Control 2
#define LCD_REG_10_PC1            0x10   // Power Control 1 
#define LCD_REG_11_PC2            0x11   // Power Control 2 
#define LCD_REG_12_PC3            0x12   // Power Control 3 
#define LCD_REG_13_PC4            0x13   // Power Control 4 
#define LCD_REG_14            0x14
#define LCD_REG_15            0x15
#define LCD_REG_16            0x16
#define LCD_REG_17            0x17
#define LCD_REG_18            0x18
#define LCD_REG_19            0x19
#define LCD_REG_1A            0x1A
#define LCD_REG_1B            0x1B
#define LCD_REG_1C            0x1C
#define LCD_REG_1D            0x1D
#define LCD_REG_1E            0x1E
#define LCD_REG_1F            0x1F
#define LCD_REG_20_GHAdd          0x20			// GRAM Horizontal Address Set
#define LCD_REG_21_GVAdd          0x21			// GRAM Vertical Address Set
#define LCD_REG_22_MWrte          0x22			// memory write
#define LCD_REG_29_PC7            0x29      // power Control 7
#define LCD_REG_2B_FrameRCC       0x2B    // Frame Rate and Color Control
#define LCD_REG_30_GC1            0x30    // Gamma Control 1 
#define LCD_REG_31_GC2            0x31    // Gamma Control 2
#define LCD_REG_32_GC3            0x32
#define LCD_REG_35_GC4            0x35
#define LCD_REG_36_GC5            0x36
#define LCD_REG_37_GC6            0x37
#define LCD_REG_38_GC7            0x38
#define LCD_REG_39_GC8            0x39
#define LCD_REG_3C_GC9            0x3C
#define LCD_REG_3D_GC10           0x3D    // Gamma Control 10
#define LCD_REG_62            0x3E
#define LCD_REG_63            0x3F
#define LCD_REG_64            0x40
#define LCD_REG_65            0x41
#define LCD_REG_66            0x42
#define LCD_REG_67            0x43
#define LCD_REG_68            0x44
#define LCD_REG_69            0x45
#define LCD_REG_70            0x46
#define LCD_REG_71            0x47
#define LCD_REG_72            0x48
#define LCD_REG_73            0x49
#define LCD_REG_74            0x4A
#define LCD_REG_75            0x4B
#define LCD_REG_76            0x4C
#define LCD_REG_77            0x4D
#define LCD_REG_78            0x4E
#define LCD_REG_79            0x4F
#define LCD_REG_50_HSAdd            0x50		// Horizontal Start Address Set
#define LCD_REG_51_HEAdd            0x51		// Horizontal End Address Set
#define LCD_REG_52_VSAdd            0x52		// Vertical Start Address Set
#define LCD_REG_53_VEAdd            0x53		// Vertical End Address Set
#define LCD_REG_60_DOC2             0x60    // Driver Output Control
#define LCD_REG_61_BIDC             0x61    //  Base Image Display Control 
#define LCD_REG_6A_VSC              0x6A  // Vertical Scroll Control 
#define LCD_REG_118           0x76
#define LCD_REG_128           0x80
#define LCD_REG_129           0x81
#define LCD_REG_130           0x82
#define LCD_REG_131           0x83
#define LCD_REG_132           0x84
#define LCD_REG_133           0x85
#define LCD_REG_134           0x86
#define LCD_REG_135           0x87
#define LCD_REG_136           0x88
#define LCD_REG_137           0x89
#define LCD_REG_139           0x8B
#define LCD_REG_140           0x8C
#define LCD_REG_141           0x8D
#define LCD_REG_143           0x8F
#define LCD_REG_144           0x90
#define LCD_REG_145           0x91
#define LCD_REG_146           0x92
#define LCD_REG_147           0x93
#define LCD_REG_148           0x94
#define LCD_REG_149           0x95
#define LCD_REG_150           0x96
#define LCD_REG_151           0x97
#define LCD_REG_152           0x98
#define LCD_REG_153           0x99
#define LCD_REG_154           0x9A
#define LCD_REG_157           0x9D
#define LCD_REG_192           0xC0
#define LCD_REG_193           0xC1
#define LCD_REG_229           0xE5

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

/* Start Initial Sequence --------------------------------------------------*/
  _writeCommand(LCD_REG_01_DOC1, 0x011C); /* Set SS and SM bit */
  _writeCommand(LCD_REG_02_lcdDC, 0x0700); /* Set 1 line inversion */
  _writeCommand(LCD_REG_03_EnMode, 0x1018); /* Set GRAM write direction and BGR=1. */
  _writeCommand(LCD_REG_04_Resize, 0x0000); /* Resize register */
  _writeCommand(LCD_REG_08_DC2, 0x0202); /* Set the back porch and front porch */
  _writeCommand(LCD_REG_09_DC3, 0x0000); /* Set non-display area refresh cycle ISC[3:0] */
  _writeCommand(LCD_REG_0A_DC4, 0x0000); /* FMARK function */
  _writeCommand(LCD_REG_0C_RGBDIC1, 0x0000); /* RGB interface setting */
  _writeCommand(LCD_REG_0D_Frame, 0x0000); /* Frame marker Position */
  _writeCommand(LCD_REG_0F_RGBDIC2, 0x0000); /* RGB interface polarity */
  /* Adjust the Gamma Curve (ILI9325) ----------------------------------------*/
//  _writeCommand(LCD_REG_30_GC1, 0x0007);
//  _writeCommand(LCD_REG_31_GC2, 0x0302);
//  _writeCommand(LCD_REG_32_GC3, 0x0105);
//  _writeCommand(LCD_REG_35_GC4, 0x0206);
//  _writeCommand(LCD_REG_36_GC5, 0x0808);
//  _writeCommand(LCD_REG_37_GC6, 0x0206);
//  _writeCommand(LCD_REG_38_GC7, 0x0504);
//  _writeCommand(LCD_REG_39_GC8, 0x0007);
//  _writeCommand(LCD_REG_3C_GC9, 0x0105);
//  _writeCommand(LCD_REG_3D_GC10, 0x0808);

/* Set GRAM area -----------------------------------------------------------*/
	_setRegion(0,0,170,220);
	
	_writeCommand(LCD_REG_60_DOC2,  0xA700); /* Gate Scan Line(GS=1, scan direction is G320~G1) */
  _writeCommand(LCD_REG_61_BIDC,  0x0001); /* NDL,VLE, REV */
  _writeCommand(LCD_REG_6A_VSC, 0x0000); /* set scrolling line */
/* Partial Display Control -------------------------------------------------*/
//  _writeCommand(LCD_REG_128, 0x0000);
//  _writeCommand(LCD_REG_129, 0x0000);
//  _writeCommand(LCD_REG_130, 0x0000);
//  _writeCommand(LCD_REG_131, 0x0000);
//  _writeCommand(LCD_REG_132, 0x0000);
//  _writeCommand(LCD_REG_133, 0x0000);
//  
//  /* Panel Control -----------------------------------------------------------*/
//  _writeCommand(LCD_REG_144, 0x0010);
//  _writeCommand(LCD_REG_146, 0x0000);
//  _writeCommand(LCD_REG_147, 0x0003);
//  _writeCommand(LCD_REG_149, 0x0110);
//  _writeCommand(LCD_REG_151, 0x0000);
//  _writeCommand(LCD_REG_152, 0x0000);

	on();
  _setIndexReg(LCD_REG_22_MWrte);
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

void Lcd::on()
{
/* Power On sequence -------------------------------------------------------*/
  _writeCommand(LCD_REG_10_PC1, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  _writeCommand(LCD_REG_11_PC2, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  _writeCommand(LCD_REG_12_PC3, 0x0000); /* VREG1OUT voltage */
  _writeCommand(LCD_REG_13_PC4, 0x0000); /* VDV[4:0] for VCOM amplitude*/

  _writeCommand(LCD_REG_10_PC1, 0x17B0); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  _writeCommand(LCD_REG_11_PC2, 0x0137); /* DC1[2:0], DC0[2:0], VC[2:0] */
  _writeCommand(LCD_REG_12_PC3, 0x0139); /* VREG1OUT voltage */
  _writeCommand(LCD_REG_13_PC4, 0x0052); /* VDV[4:0] for VCOM amplitude */
  
  _writeCommand(LCD_REG_29_PC7, 0x0013); /* VCM[4:0] for VCOMH */
// 
//  /* Display On */
//  _writeCommand(LCD_REG_07_DC1, 0x0173); /* 262K color and display ON */
	_writeCommand(LCD_REG_07_DC1, 0x0177); /* 262K color and display ON */
}
void Lcd::off()
{
  /* Power Off sequence ------------------------------------------------------*/
  _writeCommand(LCD_REG_10_PC1, 0x0000); /* SAP, BT[3:0], AP, DSTB, SLP, STB */
  _writeCommand(LCD_REG_11_PC2, 0x0000); /* DC1[2:0], DC0[2:0], VC[2:0] */
  _writeCommand(LCD_REG_12_PC3, 0x0000); /* VREG1OUT voltage */
  _writeCommand(LCD_REG_13_PC4, 0x0000); /* VDV[4:0] for VCOM amplitude*/
  
  _writeCommand(LCD_REG_29_PC7, 0x0000); /* VCM[4:0] for VCOMH */
  
  /* Display Off */
  _writeCommand(LCD_REG_07_DC1, 0x0); 

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
  set_xy(0, 0);
  _setIndexReg(LCD_REG_22_MWrte);

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
  _writeCommand(LCD_REG_20_GHAdd, x);
  _writeCommand(LCD_REG_21_GVAdd, y); 
}



void Lcd::draw_pixel(int16_t x, int16_t y, uint32_t color)
{
  if (x < 0 || x > _width) return;
  if (y < 0 || y > _height) return;
  set_xy(x,y);
//  _write(color,TFTLCD_DATA);
//	set_xy(x,y);
	  /* Prepare to write GRAM */
  _setIndexReg(LCD_REG_22_MWrte);
  /* Write 16-bit GRAM Reg */
  _writeDate(color);
}

void Lcd::fill_screen(uint32_t color)
{
  clear(color);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
void Lcd::h_disp_char8x16(int16_t x, int16_t y, uint8_t ch)
{

}
void Lcd::disp_char8x16(int16_t x, int16_t y, uint8_t ch)
{

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

}

void Lcd::draw_v_line(int16_t x0, int16_t y0, int16_t y1, uint32_t color){
	  uint16_t counter = 0;

  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  _writeCommand(LCD_REG_03_EnMode, 0x1010);
  
  /* Set Cursor */
  set_xy(x0, y0);
  
  /* Prepare to write GRAM */
  _setIndexReg(LCD_REG_22_MWrte);

  /* Fill a complete vertical line */
  for(counter = 0; counter < y1; counter++)
  {
		  /* Write 16-bit GRAM Reg */
    _writeDate(color);
  }
 
  /* set GRAM write direction and BGR = 1 */
  /* I/D=00 (Horizontal : increment, Vertical : decrement) */
  /* AM=1 (address is updated in vertical writing direction) */
  _writeCommand(LCD_REG_03_EnMode, 0x1018);  
}
void Lcd::draw_h_line(int16_t x0, int16_t y0, int16_t x1, uint32_t color){
	  
  set_xy(x0, y0);  
  /* Prepare to write GRAM */
  _setIndexReg(LCD_REG_22_MWrte);
  /* Fill a complete vertical line */
  for(uint16_t counter = 0; counter < x1; counter++)
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
  /* Horizontal GRAM Start Address */
  _writeCommand(LCD_REG_50_HSAdd, (Ypos));
  /* Horizontal GRAM End Address */
  _writeCommand(LCD_REG_51_HEAdd, (Ypos + Height - 1));
  
  /* Vertical GRAM Start Address */
  _writeCommand(LCD_REG_52_VSAdd, X_MAX_PIXEL - Xpos - Width);
  /* Vertical GRAM End Address */
  _writeCommand(LCD_REG_53_VEAdd, X_MAX_PIXEL - Xpos - 1); 
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

uint16_t Lcd::_readReg(uint16_t reg)
{
	uint16_t res;
  
  _setIndexReg(reg);
  
	_rd->set();
	res = _db->read()<<8; 
	_setWrite();
	res |= _db->read(); 
	_setWrite();
	_rd->reset();		
}
