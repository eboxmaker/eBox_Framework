/**
  ******************************************************************************
  * @file    oled_ssd1306.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GT30L32S4W_H
#define __GT30L32S4W_H

#include "ebox_core.h"
#include "spi.h"


enum FontID{
        Ascii_5x7_ID,
        Ascii_7x8_ID,
        Ascii_6x12_ID,
        Ascii_8x16_ID,
        Ascii_12x24_ID,
        Ascii_16x32_ID,
        Ascii_12_ID,
        Ascii_12_Arial_ID,
        Ascii_12_Times_New_Roman_ID,
        Ascii_16_Arial_ID,
        Ascii_16_Times_New_Roman_ID,
        Ascii_24_Arial_ID,
        Ascii_24_Times_New_Roman_ID,
        Ascii_32_Arial_ID,
        Ascii_32_Times_New_Roman_ID,
        
        
        Hz_12x12_ID,
        Hz_15x16_ID ,
        Hz_24x24_ID ,
        Hz_32x32_ID ,
        Hz_6x12_Extern_ID ,
        Hz_8x16_Extern_ID ,
        Hz_8x16_special_ID ,
        Hz_12x24_GB_Extern_ID ,
        Hz_16x32_GB_Extern_ID ,
};

    enum FontBase{
        Ascii_5x7_Base=0x1DDF80,
        Ascii_7x8_Base=0x1DE280,
        Ascii_6x12_Base=0x1DBE00,
        Ascii_8x16_Base=0x1DD780,
        Ascii_12x24_Base=0x1DFF00,
        Ascii_16x32_Base=0x1E5A50,
        Ascii_12_Base=0x1DDF80,
        Ascii_12_Arial_Base=0x1DC400,
        Ascii_12_Times_New_Roman_Base=0x1DCDC0,
        Ascii_16_Arial_Base=0x1DE580,
        Ascii_16_Times_New_Roman_Base=0x1DF240,
        Ascii_24_Arial_Base=0x1E22D0,
        Ascii_24_Times_New_Roman_Base=0x1E3E90,
        Ascii_32_Arial_Base=0x1E99D0,
        Ascii_32_Times_New_Roman_Base=0x1ECA90,
        
        
        Hz_12x12_Base = 0,
        Hz_15x16_Base = 0x2C9D0,
        Hz_24x24_Base = 0x68190,
        Hz_32x32_Base = 0XEDF00,
        Hz_6x12_Extern_Base = 0x1DBE0C,
        Hz_8x16_Extern_Base = 0x1DD790,
        Hz_8x16_special_Base = 0x1F2880,
        Hz_12x24_GB_Extern_Base = 0x1DFF30,
        Hz_16x32_GB_Extern_Base = 0x1E5A90,
    };
        typedef struct {
        FontBase base;
        int16_t width;
        int16_t hight;
        uint8_t nBytesOfChar;
        int16_t num;
    }Font_t;
        
extern Font_t FontInfoTable[];


class FontLib
{
public:
    FontLib(Gpio *cs , Gpio *clk, Gpio *si,Gpio *so);
    FontLib(Gpio *cs , Spi *spi);

    void begin();

    bool get_data(uint16_t inner_code,uint8_t font_id,eBoxCharInfo_t *info);
private:
    uint32_t char_to_addr(uint16_t inner_code,Font_t &font);
    void read_nbytes(uint32_t addr,uint8_t *buf,uint16_t nbytes);

    void write_byte(uint8_t data);
    uint8_t read_byte();


    Gpio *cs;
    Gpio *clk;
    Gpio *si;
    Gpio *so;

    Spi *spi;
    Spi::Config_t config;

};
#endif

