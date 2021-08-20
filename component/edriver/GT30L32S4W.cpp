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

//const uint32_t FontBaseTable[] = 
//{
//0x1DDF80,//        Ascii_5x7_Base=0x1DDF80,
//0x1DE280,//        Ascii_7x8_Base=0x1DE280,
//0x1DBE00,//        Ascii_6x12_Base=0x1DBE00,
//0x1DD780,//        Ascii_8x16_Base=0x1DD780,
//0x1DFF00,//        Ascii_12x24_Base=0x1DFF00,
//0x1E5A50,//        Ascii_16x32_Base=0x1E5A50,
//0x1DDF80,//        Ascii_12_Base=0x1DDF80,
//0x1DC400,//        Ascii_12_Arial_Base=0x1DC400,
//0x1DCDC0,//        Ascii_12_Times_New_Roman_Base=0x1DCDC0,
//0x1DE580,//        Ascii_16_Arial_Base=0x1DE580,
//0x1DF240,//        Ascii_16_Times_New_RomanBase=0x1DF240,
//0x1E22D0,//        Ascii_24_Arial_Base=0x1E22D0,
//0x1E3E90,//        Ascii_24_Times_New_RomanBase=0x1E3E90,
//0x1E99D0,//        Ascii_32_Arial_Base=0x1E99D0,
//0x1ECA90,//        Ascii_32_Times_New_RomanBase=0x1ECA90,
//        
//        
//0,//        Hz_12x12_Base = 0,
//0x2C9D0,//        Hz_15x16_Base = 0x2C9D0,
//0x68190,//        Hz_24x24_Base = 0x68190,
//0XEDF00,//        Hz_32x32_Base = 0XEDF00,
//0x1DBE0C,//        Hz_6x12_Extern_Base = 0x1DBE0C,
//0x1DD790,//        Hz_8x16_Extern_Base = 0x1DD790,
//0x1F2880,//        Hz_8x16_special_Base = 0x1F2880,
//0x1DFF30,//        Hz_12x24_GB_Extern_Base = 0x1DFF30,
//0x1E5A90//        Hz_16x32_GB_Extern_Base = 0x1E5A90,
//};

//        Ascii_12_Arial_ID,
//        Ascii_12_Times_New_Roman_ID,
//        Ascii_16_Arial_ID,
//        Ascii_16_Times_New_RomanID,
//        Ascii_24_Arial_ID,
//        Ascii_24_Times_New_RomanID,
//        Ascii_32_Arial_ID,
//        Ascii_32_Times_New_RomanID,
Font_t FontInfoTable[] = 
{
    /*0*/{Ascii_5x7_Base,5,7,8,96},
    /*1*/{Ascii_7x8_Base,7,8,8,96},
    /*2*/{Ascii_6x12_Base,6,12,12,96},
    /*3*/{Ascii_8x16_Base,8,16,16,96},
    /*4*/{Ascii_12x24_Base,12,24,48,96},
    /*5*/{Ascii_16x32_Base,16,32,64,96},
    /*6*/{Ascii_12_Base,8,16,16,96},
    /*7*/{Ascii_12_Arial_Base,8,16,16,96},
    /*8*/{Ascii_12_Times_New_Roman_Base,8,16,16,96},
    /*9*/{Ascii_16_Arial_Base,8,16,16,96},
    /*10*/{Ascii_16_Times_New_Roman_Base,8,16,16,96},
    /*11*/{Ascii_24_Arial_Base,8,16,16,96},
    /*12*/{Ascii_24_Times_New_Roman_Base,8,16,16,96},
    /*13*/{Ascii_32_Arial_Base,8,16,16,96},
    /*14*/{Ascii_32_Times_New_Roman_Base,8,16,16,96},


    /*15*/{Hz_12x12_Base,12,12,24,6736+846},
    /*16*/{Hz_15x16_Base,15,16,32,6736+846},
    /*17*/{Hz_24x24_Base,24,24,72,6736+846},
    /*18*/{Hz_32x32_Base,32,32,128,6736+846},
    
    

};

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
        config.mode = Spi::MODE0;
        config.bit_order = Spi::MSB;
        config.prescaler = Spi::DIV4;
        spi->begin(&config);
    }
    delay_ms(100);
    uint8_t temp[256];
    read_nbytes( 0 ,temp,255);
}


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
                Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*72 + font.base;
            else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
                Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*72 + font.base;
            break;
        case Hz_32x32_Base:

            if(MSB >=0xA1 && MSB <= 0Xa9 && LSB >=0xA1)
            Address =( (MSB - 0xA1) * 94 + (LSB - 0xA1))*128+ Hz_32x32_Base;
            else if(MSB >=0xB0 && MSB <= 0xF7 && LSB >=0xA1)
            Address = ((MSB - 0xB0) * 94 + (LSB - 0xA1)+ 846)*128+ Hz_32x32_Base;
            break;
        default:
            Address = 0xffffffff;
            break;
    }      
return Address;   
}


bool FontLib::get_data(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info)
{

    uint32_t addr;
    info->pData = (uint8_t *)malloc(FontInfoTable[font_id].nBytesOfChar);

    addr = char_to_addr(inner_code,FontInfoTable[font_id]);
    if(addr != 0xFFFFFFFF){
        read_nbytes( addr ,info->pData,FontInfoTable[font_id].nBytesOfChar);
        info->XSize = FontInfoTable[font_id].width;
        info->YSize = FontInfoTable[font_id].hight;
        info->BytesPerLine = FontInfoTable[font_id].nBytesOfChar / FontInfoTable[font_id].hight;
        
        return true;
    }
    return false;
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

