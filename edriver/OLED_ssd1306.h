#ifndef __OLED_H
#define __OLED_H
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
 *              D0   接PD6(SCL)
 *              D1   接PD7(SDA)
 *              RES  接PD4
 *              DC   接PD5
 *              CS   接PD3    		 			
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
		void OLED_WrDat(uint8_t dat);	//OLED写数据
		void OLED_WrCmd(uint8_t cmd);	//OLED写命令
		void OLED_Set_Pos(uint8_t x, uint8_t y);	// OLED 设置坐标
		u32 oled_pow(uint8_t m,uint8_t n);	//m^n
	 
		GPIO* cs_pin;	//片选
		GPIO* res_pin;
		GPIO* dc_pin;
		GPIO* scl_pin;	//D0
		GPIO* sda_pin;	//D1 
 };
 

#endif


 