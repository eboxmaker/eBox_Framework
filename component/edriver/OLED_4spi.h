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
#ifndef __OLED_SPI_H
#define __OLED_SPI_H
/*==================================================================
 *   声明库说明：
 *   版	     本：
 *   作      者：
 *   创建  日期：
 * ------------------------------------------------------------------
 *  硬件环境:
 *     	0.96寸oled，4线SPI,128*64,芯片:SSD1306
 * 	OLED 4接口演示例程(STM32系列)
 *             说明: SPI协议
 *             ----------------------------------------------------------------
 *              GND    接
 *              VCC  接5V或3.3v电源
 *              D0   接PB6(SCL)
 *              D1   接PB7(SDA)
 *              RES  接PB5
 *              DC   接PB4
 *              CS   接PB3
 *-------------------------------------------------------------------
 *  软件环境:
 *
 * ------------------------------------------------------------------
 *  [支 持 库]
 *   支持库名称：
 *   需要版本：
 *   支持库说明：
 * ------------------------------------------------------------------
 *  [版本更新]
 *   修改：
 *   修改日期：
 *   版本：
 * -------------------------------------------------------------------
 *  [使用说明]
 *
 *
 =====================================================================*/

/***********************************************************
 *                   头 文 件 配 置 区                     *
 ***********************************************************/

#include "ebox.h"

/*--------------------------------------------------------*
 *                    系 统 宏 定 义                      *
 *--------------------------------------------------------*/

//----------------------I0端口定义------------------------//


//-----------------------初始化设置-----------------------//


//-----------------------初始化配置-----------------------//


//-----------------------初始化指令-----------------------//

/*--------------------------------------------------------*
 *                    常 数 宏 定 义                      *
 *--------------------------------------------------------*/
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ( ( XLevelH & 0x0F ) * 16 + XLevelL )
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xCF
#define X_WIDTH		128
#define Y_WIDTH		64
#define SIZE		16


class Oled4Spi
{
public:
    Oled4Spi(Gpio *cs, Gpio *dc, Gpio *res, Gpio *scl_d0, Gpio *sda_d1);
    void begin();

    void init(void);
    void clear(void);
    void display_on(void);
    void display_off(void);

    void draw_point(uint8_t x,uint8_t y);

    void show_char(uint8_t x, uint8_t y, uint8_t chr,u8 Char_Size);
    void show_string(uint8_t x, uint8_t y, char *p,u8 Char_Size);
    void show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
    void show_chinese(uint8_t x, uint8_t y, uint8_t no);

    void fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    void draw_bmp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const unsigned char BMP[]);

private:
    void write_data(uint8_t dat);	//OLED写数据
    void write_cmd(uint8_t cmd);	//OLED写命令
    void set_pos(uint16_t x, uint16_t y);	// OLED 设置坐标
    uint32_t oled_pow(uint8_t m, uint8_t n);	//m^n

    Gpio *cs_pin;	//片选
    Gpio *res_pin;
    Gpio *dc_pin;
    Gpio *scl_pin;	//D0
    Gpio *sda_pin;	//D1
};
#endif

