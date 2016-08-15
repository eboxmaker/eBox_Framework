#ifndef _USE_KEY_H
#define _USE_KEY_H  1
/*==================================================================
 *   声明库说明：					                       
 *   版	     本：  V4.0                                        
 *   作      者：  施飞峰                                   
 *   创建  日期：  2016/3/20                              
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

//#define DDR_IN	        GPIOB->CRH.B.MODE13 = 0X0;\
//											  GPIOB->CRH.B.CNF13 = 2;        //设置输入	
//						
//#define DDR_OUT	        GPIOB->CRH.B.MODE13 = 0X3;\
//											  GPIOB->CRH.B.CNF13 = 0;        //DDRC  |= (1 << PC0)//设置输出		
//						
//#define PORT_L	    		PBout(13) = 0;//PORTC &=~(1 << PC0)		//置低电平
//#define PORT_H	   			PBout(13) = 1;//PORTC |= (1 << PC0)		//置高电平

//#define PIN_RED	        PBin(13)      //(PINC &  (1 << PC0))	//读电平

//#define  AM2302_DATA_IN()	   GPIOB->IDR.B.IO13//PCin(1)//PIN_RED//(GPIO_AM2302,PIN_AM2302)
// 
// 
//#define 	KEY_DDR			DDRA
//#define 	KEY_PORT		PORTA
//#define 	KEY_PIN			PINA

//#define   KEY_IO_INIT   GPIOB->CRH.B.MODE15 = 0X0;\
//											  GPIOB->CRH.B.CNF15 = 2;\
//												GPIOB->CRH.B.MODE14 = 0X0;\							
//											  GPIOB->CRH.B.CNF14 = 2;\
//												GPIOB->ODR.B.IO14 = 1;\
//												GPIOB->ODR.B.IO15 = 1;

//#define 	READ_KEY_1		GPIOB->IDR.B.IO15       //PB0
//#define 	READ_KEY_2		GPIOB->IDR.B.IO14       //PB1
//#define 	READ_KEY_3			(KEY_PIN & (1<<PB2))       //PB2
/*--------------------------------------------------------*
 *                    常 数 宏 定 义                      *
 *--------------------------------------------------------*/
#define     KEY_STATE_0         0
#define     KEY_STATE_1         1
#define     KEY_STATE_2         2
#define     KEY_STATE_3         3
#define     KEY_STATE_4         4

#define     KEY_CLICK			10    //click
#define     KEY_DBLCK		  11    //dblclick
#define		  KEY_LONPR			12    //long_press
/*--------------------------------------------------------*
 *                    功 能 宏 定 义                      *
 *--------------------------------------------------------*/
//#define   KEY_VALUE    !READ_KEY_1||!READ_KEY_2//||!READ_KEY_3

/**********************************************************
 *                     结构体定义区                       *
 **********************************************************/
//typedef struct{
//	void (* init)(void);
//	unsigned char (* con_mul)(void);
//	}KEY_T;
//extern KEY_T key;
//	
//	
class BUTTON
{
	public:
		
		BUTTON(Gpio *pin);
		void begin();
		void initialize(void);
		char mode(uint8_t key_num);
		char control_mul(void);
		uint8_t bmode;	
	
	private:		
		Gpio    *data_pin;           //arduino pin number
		uint16_t count1;
		uint16_t count2;

};

//typedef struct
//{
//	uint16_t count1;
//	uint16_t count2;
//	uint8_t fun;
//	uint8_t mode;
//}KEY_M;
//extern KEY_M KEY_STATUS[2];
/**********************************************************
 *                    全局变量声明区                      *
 **********************************************************/

/**********************************************************
 *                    函 数 声 明 区                      *
 **********************************************************/

/*
extern void key_init(void);
uchar key_con_mul(void);*/

#endif


