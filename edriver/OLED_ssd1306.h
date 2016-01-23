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
		OLED_SSD1306(GPIO* p_cs_pin,GPIO* p_res_pin, GPIO* p_dc_pin,GPIO* p_scl_pin,GPIO* p_sda_pin);
		void begin();
		void OLED_Init(void);
		void OLED_Clear(void);
		void OLED_Display_On(void);
		void OLED_Display_Off(void);
	 
		void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr);
		void OLED_ShowString(uint8_t x,uint8_t y, uint8_t *p);	
		void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size);
	 
		void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
		void OLED_Fill(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t dot);
		void OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no);
		void OLED_DrawBMP(u16 x0, u16 y0,u16 x1, u16 y1,const unsigned char BMP[]);
	 
	 private:
		void OLED_WrDat(uint8_t dat);	//OLEDд����
		void OLED_WrCmd(uint8_t cmd);	//OLEDд����
		void OLED_Set_Pos(uint8_t x, uint8_t y);	// OLED ��������
		u32 oled_pow(uint8_t m,uint8_t n);	//m^n
	 
		GPIO* cs_pin;	//Ƭѡ
		GPIO* res_pin;
		GPIO* dc_pin;
		GPIO* scl_pin;	//D0
		GPIO* sda_pin;	//D1 
 };
 

#endif
