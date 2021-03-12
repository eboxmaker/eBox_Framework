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
#ifndef __OLED_SSD1322_H
#define __OLED_SSD1322_H
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

#include "ebox_core.h"
#include "spi.h"
#include "interface/hmi.h"


//-----------------------初始化指令-----------------------//

/*--------------------------------------------------------*
 *                    常 数 宏 定 义                      *
 *--------------------------------------------------------*/
#define XLevelL		0x00
#define XLevelH		0x10
#define XLevel	    ( ( XLevelH & 0x0F ) * 16 + XLevelL )
#define Max_Column	256
#define Max_Row		64
#define	Brightness	0xCF
#define SSD1322_LCDWIDTH		256
#define SSD1322_LCDHEIGHT		64
#define SIZE		16
#define BITS_PER_PIXEL  4


#define SSD1322_256_64_4
//#define SSD1322_256_64_1


//#define BLACK 0x00
//#define WHITE 0x0F
//#define INVERSE 2

class OledSSD1322:public HMI
{
public:
    OledSSD1322(Gpio *cs, Gpio *dc, Gpio *res, Gpio *scl_d0, Gpio *sda_d1);

    OledSSD1322(Gpio *cs, Gpio *dc,Gpio *res , Spi *spi);

    void begin();

    virtual void    draw_pixel(int16_t x, int16_t y,uint32_t color);
    virtual void    fill_screen(uint32_t color);
    virtual void    flush();

    void clear();
    void display_on(void);
    void display_off(void);

private:
    void init(void);

    void write_data_hard(uint8_t data);	//OLED写数据
    void write_cmd_hard(uint8_t cmd);	//OLED写命令  

    void write_data_soft(uint8_t dat);	//OLED写数据
    void write_cmd_soft(uint8_t cmd);	//OLED写命令

    void write_data(uint8_t dat);	//OLED写数据
    void write_cmd(uint8_t cmd);	//OLED写命令

    void set_column_addr(uint16_t x0, uint16_t x1);	// OLED 设置坐标
    void set_row_addr(uint16_t y0, uint16_t y1);	// OLED 设置坐标

    Gpio *cs_pin;	//片选
    Gpio *res_pin;
    Gpio *dc_pin;
    Gpio *scl_pin;	//D0
    Gpio *sda_pin;	//D1

    Spi *spi;
    Spi::Config_t config;
    
public:
    int16_t width;
    int16_t hight;
    uint16_t buffer_size;

};
#endif

