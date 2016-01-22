/*===========================================================================
 *  ��������:                         	   
 *  Դ�ļ���:                                                        	   
 *  ��������:                                                                                                                  
 *  ��    ��:                                                               
 *  ��	  վ�� 														         
 *  �汾˵��:   														 
 *--------------------------------------------------------------------------- 
 *  Ӳ������:                                                                                 
 *     	0.96��oled��4��SPI,128*64,оƬ:SSD1306	
 * 	OLED_SSD1306 4�ӿ���ʾ����(STM32ϵ��)
 *             ˵��: SPIЭ��
 *             ----------------------------------------------------------------
 *              GND    ��
 *              VCC  ��5V��3.3v��Դ
 *              D0   ��PD6(SCL)
 *              D1   ��PD7(SDA)
 *              RES  ��PD4
 *              DC   ��PD5
 *              CS   ��PD3  
 *--------------------------------------------------------------------------- 
 *  �������:                                                                                                  
 *     		             					
 *---------------------------------------------------------------------------                                                    
 *  ��������:                                                                        
 *                                
 *                                                 							                                                                     
 *--------------------------------------------------------------------------- 
 * ����Ȩ��  Copyright(C)             All Rights Reserved  				          
 * ��������  �˳������ڲ��ԣ�������ע����Ȩ��������Ϣ��                    
 *===========================================================================*/

/***********************************************************
 *                  ͷ �� �� �� �� ��                     *
 ***********************************************************/
#include "oled_ssd1306.h"
#include "font.h"


/**********************************************************
 *                    �� �� �� �� ��                      *
 **********************************************************/
OLED_SSD1306::OLED_SSD1306(GPIO* p_cs_pin,GPIO* p_res_pin, GPIO* p_dc_pin,GPIO* p_scl_pin,GPIO* p_sda_pin)
{
	cs_pin		= p_cs_pin;
	res_pin 	= p_res_pin;
	dc_pin 		= p_dc_pin;
	scl_pin 	= p_scl_pin;
	sda_pin 	= p_sda_pin;	
}
void OLED_SSD1306::begin(void)	//��ʼ��SSD1306	
{
	res_pin->mode(OUTPUT_PP);
	dc_pin->mode(OUTPUT_PP);
	scl_pin->mode(OUTPUT_PP);
	sda_pin->mode(OUTPUT_PP);
	cs_pin->mode(OUTPUT_PP);
}

/***********************************************************
*   �������ƣ�
*   ����������   OLEDд����
*   �����б�
*   ���ؽ����
*   ��    ע��
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
*   �������ƣ�
*   ����������   OLEDд����
*   �����б�
*   ���ؽ����
*   ��    ע��
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
*   �������ƣ�
*   ����������   OLED��������
*   �����б�
*   ���ؽ����
*   ��    ע��
***********************************************************/
void OLED_SSD1306::
OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WrCmd(0xb0+y);
	OLED_WrCmd(((x&0xf0)>>4)|0x10);
	OLED_WrCmd((x&0x0f)|0x01);
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED����
*   �����б�
*   ���ؽ����
*   ��    ע����������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
***********************************************************/
void OLED_SSD1306::OLED_Clear(void)
{
	uint8_t i,n;		    
	for(i=0;i<8;i++)  
	{  
		OLED_WrCmd(0xb0+i);    //����Ҳ��ַ(0~7)
		OLED_WrCmd (0x00);      //������ʾλ�á��е͵�ַ
		OLED_WrCmd (0x10);      //������ʾλ�á��иߵ�ַ   
		for(n=0;n<128;n++)OLED_WrDat(0); 
	} //������ʾ
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED_SSD1306������ʾ
*   �����б�
*   ���ؽ����
*   ��    ע��
***********************************************************/
void OLED_SSD1306::OLED_Display_On(void)
{
	OLED_WrCmd(0x8d);	//SET DCDC����
	OLED_WrCmd(0X14);  //DCDC ON
	OLED_WrCmd(0XAF);  //DISPLAY ON
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED_SSD1306�ر���ʾ
*   �����б�
*   ���ؽ����
*   ��    ע��
***********************************************************/
void OLED_SSD1306::OLED_Display_Off(void)
{
	OLED_WrCmd(0x8d);	//SET DCDC����
	OLED_WrCmd(0X10);  //DCDC ON
	OLED_WrCmd(0XAE);  //DISPLAY ON
}

/***********************************************************
*   �������ƣ�
*   ����������   OLED��ʾ�����ַ�
*   �����б�
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ���ַ�,���������ַ�
***********************************************************/
void OLED_SSD1306::OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr)
{
		unsigned char c=0,i=0;	
		c=chr-' ';//�õ�ƫ�ƺ��ֵ			
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
*   �������ƣ�
*   ����������   OLED��ʾһ���ַ���
*   �����б�
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ���ַ���
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
*   �������ƣ�
*   ����������   OLED��ʾһ���ַ���
*   �����б�len :���ֵ�λ����size:�����С��mode:ģʽ	0,���ģʽ;1,����ģʽ
*			  num:��ֵ(0~4294967295);	
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ���ַ���
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
*   �������ƣ�
*   ����������   OLED��ʾһ������
*   �����б�
*   ���ؽ����
*   ��    ע����ָ��λ����ʾһ������
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
*   �������ƣ�
*   ������������ʾBMPͼƬ128��64��ʼ������(x,y),
*   �����б�x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7
*   ���ؽ����
*   ��    ע��
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
*   �������ƣ�
*   ����������   OLED_SSD1306��ʼ��
*   �����б�
*   ���ؽ����
*   ��    ע��
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
	OLED_WrCmd(0xa1);//--Set SEG/Column Mapping     0xa0���ҷ��� 0xa1����
	OLED_WrCmd(0xc8);//Set COM/Row Scan Direction   0xc0���·��� 0xc8����
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

