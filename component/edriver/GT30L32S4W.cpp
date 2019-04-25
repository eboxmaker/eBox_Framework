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
#include "gt30l32s4w.h"
#include "ebox.h"
Font_t Ascii8x16 = {Ascii_8x16_Base,8,16,16,96};
Font_t Hz32x32 = {Hz_32x32_Base,32,32,128,6736};

FontLib::FontLib(Gpio *cs , Gpio *clk, Gpio *si,Gpio *so)
{
    this->cs = cs;
    this->clk = clk;
    this->si = si;
    this->so = so;
    this->spi = NULL;
}

FontLib::FontLib(Gpio *cs , Spi *spi)
{
    
    this->cs = cs;
    this->spi = spi;
}
void FontLib::begin()
{
    cs->mode(OUTPUT_PP);
    
    if(this->spi == NULL)
    {
        clk->mode(OUTPUT_PP);
        si->mode(OUTPUT_PP);
        so->mode(INPUT_PD);
    }
    else
    {
        config.dev_num = cs->id;
        config.mode = SPI_MODE0;
        config.bit_order = MSB_FIRST;
        config.prescaler = SPI_CLOCK_DIV4;
        spi->begin(&config);
    }
    uint8_t temp[16];
    read_nbytes( 0 ,temp,16);
}

//        Ascii_5x7_Base=0x1DDF80,
//        Ascii_7x8_Base=0x1DE280,
//        Ascii_6x12_Base=0x1DBE00,
//        Ascii_8x16_Base=0x1DD780,
//        Ascii_12x24_Base=0x1DFF00,
//        Ascii_16x32_Base=0x1E5A50,
//        Ascii_12_Base=0x1DDF80,
//        Ascii_12_Arial_Base=0x1DC400,
//        Ascii_12_Times_New_Roman_Base=0x1DCDC0,
//        Ascii_16_Arial_Base=0x1DE580,
//        Ascii_16_Times_New_RomanBase=0x1DF240,
//        Ascii_24_Arial_Base=0x1E22D0,
//        Ascii_24_Times_New_RomanBase=0x1E3E90,
//        Ascii_32_Arial_Base=0x1E99D0,
//        Ascii_32_Times_New_RomanBase=0x1ECA90,
//        
//        
//        Hz_12x12_Base = 0,
//        Hz_15x16_Base = 0x2C9D0,
//        Hz_24x24_Base = 0x68190,
//        Hz_32x32_Base = 0XEDF00,
//        Hz_6x12_Extern_Base = 0x1DBE0C,
//        Hz_8x16_Extern_Base = 0x1DD790,
//        Hz_8x16_special_Base = 0x1F2880,
//        Hz_12x24_GB_Extern_Base = 0x1DFF30,
//        Hz_16x32_GB_Extern_Base = 0x1E5A90,
uint32_t FontLib::char_to_addr(uint16_t inner_code,Font_t &font)
{
    uint32_t Address;
    uint8_t MSB,LSB;
    MSB = inner_code>>8;
    LSB = inner_code&0xff;
    switch((uint32_t)font.base)
    {
        case Ascii_5x7_Base:
        case Ascii_7x8_Base:
            if (inner_code >= 0x20 && inner_code <= 0x7E)
                Address = (inner_code - 0x20 ) * 8 + font.base;      
            break; 
        case Ascii_6x12_Base:
            if (inner_code >= 0x20 && inner_code <= 0x7E)
                Address = (inner_code - 0x20 ) * 12 + font.base;      
            break;
        case Ascii_8x16_Base:
            if (inner_code >= 0x20 && inner_code <= 0x7E)
                Address = ((inner_code - 0x20 ) << 4) + font.base;
            break;       
        case Ascii_12x24_Base:
            if (inner_code >= 0x20 && inner_code <= 0x7E)
                Address = (inner_code - 0x20 ) * 48 + font.base;      
            break;
        case Ascii_16x32_Base:
            if (inner_code >= 0x20 && inner_code <= 0x7E)
                Address = (inner_code - 0x20 ) * 64 + font.base;      
            break;



        case Hz_12x12_Base:
            if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
                Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*24 + font.base;
            else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
                Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*24 + font.base;
            break;
        case Hz_15x16_Base:
            if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
                Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32 + font.base;
            else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
                Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32 + font.base;
            break;
        case Hz_24x24_Base:
            if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
                Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*32 + font.base;
            else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
                Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*32 + font.base;
            break;
        case Hz_32x32_Base:

            if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
            Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*128+ Hz_32x32_Base;
            else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
            Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*128+ Hz_32x32_Base;
            break;
    }      
return Address;   
}
void FontLib::get_char_buf(uint16_t inner_code,Font_t &font,uint8_t *p)
{
    uint8_t temp[256];
    uint32_t addr;
    addr = char_to_addr(inner_code,font);
    read_nbytes( addr ,temp,font.nBytesOfChar);
    memcpy(p,temp,font.nBytesOfChar);

}
void FontLib::get_one_hz_frame(const char *str,Font_t &font,Frame_t &frame)
{
    uint16_t code = 0;
    uint32_t addr;
    uint8_t buf[200];
        frame.len = 0;
    frame.width = 0;
    while(*str)
    {
        if(*str < 0x7e)//是字母
            code = *str++;
        else//汉字
        {
            code = (*str++) << 8;
            code += *str++;
        }
                    
        addr = char_to_addr(code,font);
        read_nbytes( addr ,&buf[frame.len],font.nBytesOfChar);
        
        frame.len += font.nBytesOfChar;
        
        frame.width += font.width;
        frame.hight = font.hight;
//        uart1.print("addr");
//        uart1.println(addr,HEX);        
    }
    memcpy(frame.ptr,buf,font.nBytesOfChar);
//    
//    for(int i = 0; i < frame.hight; i++) 
//    {
//        for(int j = 0; j < ybytes; j++)
//        {
//            frame.ptr[i*ybytes + j] = buf[j*font.hight + i];
//        }
//    }
}

void FontLib::get_str_frame(const char *str,Font_t &font,Frame_t &frame)
{
    uint16_t code = 0;
    uint32_t addr;
    uint8_t buf[200];
        frame.len = 0;
    frame.width = 0;
    while(*str)
    {
        if(*str < 0x7e)//是字母
            code = *str++;
        else//汉字
        {
            code = (*str++) << 8;
            code += *str++;
        }
                    
        addr = char_to_addr(code,font);
        read_nbytes( addr ,&buf[frame.len],font.nBytesOfChar);
        
        frame.len += font.nBytesOfChar;
        
        frame.width += font.width;
        frame.hight = font.hight;
//        uart1.print("addr");
//        uart1.println(addr,HEX);        
    }
    int ybytes = frame.width/font.width;
    
    for(int i = 0; i < frame.hight; i++) 
    {
        for(int j = 0; j < ybytes; j++)
        {
            frame.ptr[i*ybytes + j] = buf[j*font.hight + i];
        }
    }
    
}






void FontLib::write_byte(uint8_t data)
{
    unsigned char  i;
	 
	for(i=0;i<8;i++ )
	{
		if(data&0x80)
        {
            si->set(); 
        }
		else
        {
            si->reset();
        }
        data = data<<1;
        clk->reset();  //字库时钟拉低 
        clk->set();  //字库时钟拉高
    }
}

unsigned char FontLib::read_byte()
{
	unsigned char  i;
	unsigned char  ret_data=0; //返回数据初始化
	clk->set();//字库时钟拉高
	so->mode(INPUT_PD);  //下拉输入
	for(i=0;i<8;i++)
	{
		si->set(); 
		clk->reset();  //字库时钟拉低  
		ret_data=ret_data<<1;
		if( so->read() )
        {
            ret_data=ret_data+1; 
        }
		else
        {
            ret_data=ret_data+0; 
        }
		clk->set(); //字库时钟拉高
	}
	//GBZK_ROMOUTRESET();	 //开漏输出
	return(ret_data); //返回读出的一个字节

}

void FontLib::read_nbytes(uint32_t addr,uint8_t *buf,uint16_t nbytes)
{
	
    cs->reset();
    
    delay_us(10);
    
    if(this->spi == NULL)
    {
        write_byte(0X03);
        write_byte((addr & 0x00FF0000) >> 16);
        write_byte((addr & 0x0000FF00) >> 8);
        write_byte((addr & 0x00FF00FF) >> 0);
        for(int i = 0; i < nbytes; i++)
            *(buf+i) = read_byte();
        
    }else{
        spi->take(&config);
        spi->transfer(0x03);
        spi->transfer((addr & 0x00FF0000) >> 16);
        spi->transfer((addr & 0x0000FF00) >> 8);
        spi->transfer((addr & 0x00FF00FF) >> 0);
        for(int i = 0; i < nbytes; i++)
            buf[i] = spi->read();
        spi->release();
    }
    cs->set(); 
    delay_us(10);    
}

