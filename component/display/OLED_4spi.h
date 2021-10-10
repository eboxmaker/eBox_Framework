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
 *              D0   ��PB6(SCL)
 *              D1   ��PB7(SDA)
 *              RES  ��PB5
 *              DC   ��PB4
 *              CS   ��PB3
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

#include "ebox_core.h"

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


class Oled4Spi
{
public:
    Oled4Spi(Gpio *cs, Gpio *dc, Gpio *res, Gpio *scl_d0, Gpio *sda_d1);
    void begin();

    void init(void);
    void clear(void);
    void display_on(void);
    void display_off(void);

    void draw_point(uint8_t x, uint8_t y);

    void show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
    void show_string(uint8_t x, uint8_t y, char *p, uint8_t Char_Size);
    void show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size);
    void show_chinese(uint8_t x, uint8_t y, uint8_t no);

    void fill(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t dot);
    void draw_bmp(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, const unsigned char BMP[]);

private:
    void write_data(uint8_t dat);	//OLEDд����
    void write_cmd(uint8_t cmd);	//OLEDд����
    void set_pos(uint16_t x, uint16_t y);	// OLED ��������
    uint32_t oled_pow(uint8_t m, uint8_t n);	//m^n

    Gpio *cs_pin;	//Ƭѡ
    Gpio *res_pin;
    Gpio *dc_pin;
    Gpio *scl_pin;	//D0
    Gpio *sda_pin;	//D1
};
#endif

