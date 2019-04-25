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
}
uint32_t FontLib::char_to_addr(uint16_t inner_code,Font_t &font)
{
    uint32_t addr;
   switch((uint32_t)font.base)
   {
       case Ascii_8x16_Base:
           addr = (inner_code - 0x20 ) * 16 + font.base;
           break;
   }      
return addr;   
}

void FontLib::get_str_frame(const char *str,Font_t &font,Frame_t &frame)
{
    uint16_t code = 0;
    uint32_t addr;
    
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
        read_nbytes( addr ,&frame.ptr[frame.len],font.size);
        frame.len += font.size;
        
        uart1.print("addr");
        uart1.println(addr,HEX);
        
        
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
}

