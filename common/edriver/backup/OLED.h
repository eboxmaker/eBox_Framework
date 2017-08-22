/**
  ******************************************************************************
  * @file    oled.h
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
#ifndef _USE_OLED_H
#define _USE_OLED_H
/*==================================================================
 *   声明库说明：
 *   版	     本：
 *   作      者：
 *   创建  日期：
 * ------------------------------------------------------------------
 *  硬件环境:
 *
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


/*--------------------------------------------------------*
 *                    动 作 宏 定 义                      *
 *--------------------------------------------------------*/


/**********************************************************
 *                     结构体定义区                       *
 **********************************************************/
class OLED
{
public:
    OLED(Gpio *p_res_pin, Gpio *p_dc_pin, Gpio *p_scl_pin, Gpio *p_sda_pin);
    void begin();
    void OLED_Fill(unsigned char bmp_dat);
    void OLED_CLS(void);
    void OLED_init(void);
    void OLED_P6x8Str(unsigned char x, unsigned char y, unsigned char *ch);
    void OLED_P8x16Str(unsigned char x, unsigned char y, unsigned char *ch);
    void OLED_P16x16Ch(unsigned char x, unsigned char y, unsigned char N);
    void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char *bmp);
    void OLED_P6x8p(unsigned char x, unsigned char y, unsigned char *p, unsigned int len);
    void OLED_f6x8int(unsigned char x, unsigned char y, unsigned int data);
    void OLED_P16x32num(unsigned char x, unsigned char y, unsigned char *bnum);
    void OLED_P8x16ASCII(unsigned char x, unsigned char y, unsigned char ch);

private:
    void OLED_WrDat(unsigned char dat);
    void OLED_WrCmd(unsigned char cmd);
    void OLED_Set_Pos(unsigned char x, unsigned char y);

    Gpio *res_pin;
    Gpio *dc_pin;
    Gpio *scl_pin;
    Gpio *sda_pin;
};


/**********************************************************
 *                    全局变量声明区                      *
 **********************************************************/

/**********************************************************
 *                    函 数 声 明 区                      *
 **********************************************************/


#endif

