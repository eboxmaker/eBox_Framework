/*===========================================================================
 *  程序名称:                         	   
 *  源文件名:                                                        	   
 *  创建日期:                                                                                                                  
 *  作    者:                                                               
 *  网	  站： 														         
 *  版本说明:   														 
 *--------------------------------------------------------------------------- 
 *  硬件环境:                                                                                 
 *     	0.96寸oled，4线SPI,128*64,芯片:SSD1306	
 * 	OLED_SSD1306 4接口演示例程(STM32系列)
 *             说明: SPI协议
 *             ----------------------------------------------------------------
 *              GND    接
 *              VCC  接5V或3.3v电源
 *              D0   接PD6(SCL)
 *              D1   接PD7(SDA)
 *              RES  接PD4
 *              DC   接PD5
 *              CS   接PD3  
 *--------------------------------------------------------------------------- 
 *  软件环境:                                                                                                  
 *     		             					
 *---------------------------------------------------------------------------                                                    
 *  内容描述:                                                                        
 *                                
 *                                                 							                                                                     
 *--------------------------------------------------------------------------- 
 * 【版权】  Copyright(C)             All Rights Reserved  				          
 * 【声明】  此程序用于测试，引用请注明版权和作者信息！                    
 *===========================================================================*/

/***********************************************************
 *                  头 文 件 配 置 区                     *
 ***********************************************************/
#include "oled_ssd1306.h"
#include "font.h"


/**********************************************************
 *                    函 数 声 明 区                      *
 **********************************************************/
OLED_SSD1306::OLED_SSD1306(GPIO* p_cs_pin,GPIO* p_res_pin, GPIO* p_dc_pin,GPIO* p_scl_pin,GPIO* p_sda_pin)
{
	cs_pin		= p_cs_pin;
	res_pin 	= p_res_pin;
	dc_pin 		= p_dc_pin;
	scl_pin 	= p_scl_pin;
	sda_pin 	= p_sda_pin;	
}
void OLED_SSD1306::begin(void)	//初始化SSD1306	
{
	res_pin->mode(OUTPUT_PP);
	dc_pin->mode(OUTPUT_PP);
	scl_pin->mode(OUTPUT_PP);
	sda_pin->mode(OUTPUT_PP);
	cs_pin->mode(OUTPUT_PP);
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::OLED_WrDat(uint8_t dat)
{
	uint8_t i;
	dc_pin->set();
	cs_pin->reset();
	for(i=0;i<8;i++)
	{
		scl_pin->reset();
		if(dat&0x80)
			sda_pin->set();
		else
			sda_pin->reset();
		scl_pin->set();
		dat<<=1;	
	}
	cs_pin->set();
	dc_pin->set();
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED写数据
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::OLED_WrCmd(uint8_t cmd)
{
	uint8_t i;
	dc_pin->reset();
	cs_pin->reset();
	for(i=0;i<8;i++)
	{
		scl_pin->reset();
		if(cmd&0x80)
			sda_pin->set();
		else
			sda_pin->reset();
		scl_pin->set();
		cmd<<=1;	
	}
	cs_pin->set();
	dc_pin->set();
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED设置坐标
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::
OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED清屏
*   参数列表：
*   返回结果：
*   备    注：清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!
***********************************************************/
void OLED_SSD1306::OLED_Clear(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WrCmd(0xb0+i);    //设置也地址(0~7)
		OLED_WrCmd (0x00);      //设置显示位置—列低地址
		OLED_WrCmd (0x10);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)OLED_WrDat(0); 
	} //更新显示
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED_SSD1306开启显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::OLED_Display_On(void)
{
	OLED_WrCmd(0x8d);	//SET DCDC命令
	OLED_WrCmd(0X14);  //DCDC ON
	OLED_WrCmd(0XAF);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED_SSD1306关闭显示
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::OLED_Display_Off(void)
{
	OLED_WrCmd(0x8d);	//SET DCDC命令
	OLED_WrCmd(0X10);  //DCDC ON
	OLED_WrCmd(0XAE);  //DISPLAY ON
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示单个字符
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个字符,包括部分字符
***********************************************************/
void OLED_SSD1306::OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{
		unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
		{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
				OLED_WrDat(F8X16[c*16+i]);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
				OLED_WrDat(F8X16[c*16+i+8]);
		}
		else 
		{	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
					OLED_WrDat(F6x8[c][i]);	
		}
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个字符串
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个字符串
***********************************************************/
void OLED_SSD1306::OLED_ShowString(uint8_t x,uint8_t y, uint8_t *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个字符串
*   参数列表：len :数字的位数，size:字体大小，mode:模式	0,填充模式;1,叠加模式
*			  num:数值(0~4294967295);	
*   返回结果：
*   备    注：在指定位置显示一个字符串
***********************************************************/
void OLED_SSD1306::OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size)
{
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED显示一个汉字
*   参数列表：
*   返回结果：
*   备    注：在指定位置显示一个汉字
***********************************************************/
void OLED_SSD1306::OLED_ShowCHinese(uint8_t x,uint8_t y,uint8_t no)
{
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				OLED_WrDat(Hzk[2*no][t]);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				OLED_WrDat(Hzk[2*no+1][t]);
				adder+=1;
      }	
}


/***********************************************************
*   函数名称：
*   功能描述：显示BMP图片128×64起始点坐标(x,y),
*   参数列表：x的范围0～127，y为页的范围0～7
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::OLED_DrawBMP(u16 x0, u16 y0,u16 x1, u16 y1,const unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	OLED_WrDat(BMP[j++]);	    	
	    }
	}
} 

/*****************  m^n  ********************/
uint32_t OLED_SSD1306::oled_pow(uint8_t m,uint8_t n)
{
	uint32_t result=1;	 
	while(n--)result*=m;    
	return result;
}

/***********************************************************
*   函数名称：
*   功能描述：   OLED_SSD1306初始化
*   参数列表：
*   返回结果：
*   备    注：
***********************************************************/
void OLED_SSD1306::OLED_Init(void)
{
	begin();
	res_pin->set();
	delay_ms(100);
	res_pin->reset();
	delay_ms(100);
	res_pin->set();
	
	OLED_WrCmd(0xae);//--turn off oled panel
	OLED_WrCmd(0x00);//---set low column address
	OLED_WrCmd(0x10);//---set high column address
	OLED_WrCmd(0x40);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);//--set contrast control register
	OLED_WrCmd(0xcf); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	OLED_WrCmd(0xa6);//--set normal display
	OLED_WrCmd(0xa8);//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);//--1/64 duty
	OLED_WrCmd(0xd3);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);//-not offset
	OLED_WrCmd(0xd5);//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);//--set pre-charge period
	OLED_WrCmd(0xf1);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb);//--set vcomh
	OLED_WrCmd(0x40);//Set VCOM Deselect Level
	OLED_WrCmd(0x20);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02);//
	OLED_WrCmd(0x8d);//--set Charge Pump enable/disable
	OLED_WrCmd(0x14);//--set(0x10) disable
	OLED_WrCmd(0xa4);// Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6);// Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf);//--turn on oled panel
	
	OLED_WrCmd(0xaf);	/*display ON*/ 
	OLED_Clear();
	OLED_Set_Pos(0,0); 
}

