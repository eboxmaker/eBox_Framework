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
#include "spi.h"
#include "interface/hmi.h"


//-----------------------��ʼ��ָ��-----------------------//

/*--------------------------------------------------------*
 *                    �� �� �� �� ��                      *
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

    void write_data_hard(uint8_t data);	//OLEDд����
    void write_cmd_hard(uint8_t cmd);	//OLEDд����  

    void write_data_soft(uint8_t dat);	//OLEDд����
    void write_cmd_soft(uint8_t cmd);	//OLEDд����

    void write_data(uint8_t dat);	//OLEDд����
    void write_cmd(uint8_t cmd);	//OLEDд����

    void set_column_addr(uint16_t x0, uint16_t x1);	// OLED ��������
    void set_row_addr(uint16_t y0, uint16_t y1);	// OLED ��������

    Gpio *cs_pin;	//Ƭѡ
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

