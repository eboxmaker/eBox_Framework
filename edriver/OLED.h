#ifndef _USE_OLED_H
#define _USE_OLED_H
/*==================================================================
 *   ������˵����
 *   ��	     ����
 *   ��      �ߣ�
 *   ����  ���ڣ�
 * ------------------------------------------------------------------
 *  Ӳ������:
 *
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


/*--------------------------------------------------------*
 *                    �� �� �� �� ��                      *
 *--------------------------------------------------------*/


/**********************************************************
 *                     �ṹ�嶨����                       *
 **********************************************************/
class OLED
{
public:
    OLED(GPIO *p_res_pin, GPIO *p_dc_pin, GPIO *p_scl_pin, GPIO *p_sda_pin);
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

    GPIO *res_pin;
    GPIO *dc_pin;
    GPIO *scl_pin;
    GPIO *sda_pin;
};


/**********************************************************
 *                    ȫ�ֱ���������                      *
 **********************************************************/

/**********************************************************
 *                    �� �� �� �� ��                      *
 **********************************************************/


#endif

