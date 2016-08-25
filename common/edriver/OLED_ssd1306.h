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
#ifndef __OLED_H
#define __OLED_H
/*==================================================================
 *   ������˵����
 *   ��	     ����
 *   ��      �ߣ�
 *   ����  ���ڣ�
 * ------------------------------------------------------------------
 *  Ӳ������:
 *     	0.96��oled��4��SPI,128*64,оƬ:SSD1306
 * 	OLED 4�ӿ���ʾ����(STM32ϵ��)
 *             ˵��: SPIЭ��
 *             ----------------------------------------------------------------
 *              GND    ��
 *              VCC  ��5V��3.3v��Դ
 *              D0   ��PD6(SCL)
 *              D1   ��PD7(SDA)
 *              RES  ��PD4
 *              DC   ��PD5
 *              CS   ��PD3
 *-------------------------------------------------------------------
 *  �������:
 *
 * ------------------------------------------------------------------
 *  [֧ �� ��]
 *   ֧�ֿ����ƣ�
 *   ��Ҫ�汾��
 *   ֧�ֿ�˵����
 * ------------------------------------------------------------------
 *  [�汾����]
 *   �޸ģ�
 *   �޸����ڣ�
 *   �汾��
 * -------------------------------------------------------------------
 *  [ʹ��˵��]
 *
 *
 =====================================================================*/

/***********************************************************
 *                   ͷ �� �� �� �� ��                     *
 ***********************************************************/

#include "ebox.h"

/*--------------------------------------------------------*
 *                    ϵ ͳ �� �� ��                      *
 *--------------------------------------------------------*/

//----------------------I0�˿ڶ���------------------------//


//-----------------------��ʼ������-----------------------//


//-----------------------��ʼ������-----------------------//


//-----------------------��ʼ��ָ��-----------------------//

/*--------------------------------------------------------*
 *                    �� �� �� �� ��                      *
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


class OLED_SSD1306
{
public:
    OLED_SSD1306(Gpio *cs_pin, Gpio *res_pin, Gpio *dc_pin, Gpio *scl_pin, Gpio *sda_pin);
    void begin();
    void init(void);
    void clear(void);
    void display_on(void);
    void display_off(void);

    void show_char(uint8_t x, uint8_t y, uint8_t chr);
    void show_string(uint8_t x, uint8_t y, uint8_t *p);
    void show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);

    void draw_point(uint8_t x, uint8_t y, uint8_t t);
    void fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    void show_chinese(uint8_t x, uint8_t y, uint8_t no);
    void draw_bmp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const unsigned char BMP[]);

private:
    void write_data(uint8_t dat);	//OLEDд����
    void write_cmd(uint8_t cmd);	//OLEDд����
    void set_xy(uint16_t x, uint16_t y);	// OLED ��������
    uint32_t oled_pow(uint8_t m, uint8_t n);	//m^n

    Gpio *cs_pin;	//Ƭѡ
    Gpio *res_pin;
    Gpio *dc_pin;
    Gpio *scl_pin;	//D0
    Gpio *sda_pin;	//D1
};
#endif

