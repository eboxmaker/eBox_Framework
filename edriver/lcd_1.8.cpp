/*******************************
1.8��Һ����Ļ��������
����IC:ST7735S
�ӿڣ�SPI��MODE2��3��
�ֱ��ʣ�128*160
ɫ�ʣ�256K
*******************************/
#include "lcd_1.8.h"
#include "lcd_font.h"

#define X_MAX_PIXEL	        128
#define Y_MAX_PIXEL	        160

//LCD lcd(&PB13,&PB15,&PA13,&PA15,&PA14,&PA12);

void LCD::begin(u8 dev_num)
{
    cs->mode(OUTPUT_PP);
    led->mode(OUTPUT_PP);
    rs->mode(OUTPUT_PP);
    rst->mode(OUTPUT_PP);
    
    spi_dev_lcd.mode = SPI_MODE2;
    spi_dev_lcd.bit_order = MSB_FIRST;
    spi_dev_lcd.dev_num = dev_num;
    spi_dev_lcd.prescaler = SPI_CLOCK_DIV2;
    spi->begin(&spi_dev_lcd);
    
    init();
}
//��SPI���ߴ���һ��8λ����
//void  LCD::spi_write_data(u8 Data)
//{
//	unsigned char i=0;
//	for(i=8;i>0;i--)
//	{
//		if(Data&0x80)	
//	  sda->set(); //�������
//      else sda->reset();
//	   
//      scl->reset();       
//      scl->set();
//      Data<<=1; 
//	}    
//}
//��Һ����дһ��8λָ��
void LCD::write_index(u8 Index)
{
   //SPI д����ʱ��ʼ
   cs->reset();
   rs->reset();
   spi->write(Index);
   cs->set();
}
//��Һ����дһ��8λ����
void LCD::write_data_8bit(u8 Data)
{
   cs->reset();
   rs->set();
   spi->write(Data);
   cs->set(); 
}
//��Һ����дһ��16λ����
void LCD::write_data_16bit(u16 Data)
{
    cs->reset();
    rs->set();
    spi->write(Data>>8); 	//д���8λ����
    spi->write(Data); 			//д���8λ����
    cs->set(); 
}

void LCD::write_reg(u8 Index,u8 Data)
{
    write_index(Index);
    write_data_8bit(Data);
}

void LCD::reset(void)
{
	rst->reset();
	delay_ms(1000);
	rst->set();
	delay_ms(50);
}
//LCD Init For 1.44Inch LCD Panel with ST7735R.
void LCD::init(void)
{	
	reset(); //Reset before LCD Init.

	//LCD Init For 1.44Inch LCD Panel with ST7735R.
	write_index(0x11);//Sleep exit 
	delay_ms (120);
		
	//ST7735R Frame Rate
	write_index(0xB1); 
	write_data_8bit(0x01); 
	write_data_8bit(0x2C); 
	write_data_8bit(0x2D); 

	write_index(0xB2); 
	write_data_8bit(0x01); 
	write_data_8bit(0x2C); 
	write_data_8bit(0x2D); 

	write_index(0xB3); 
	write_data_8bit(0x01); 
	write_data_8bit(0x2C); 
	write_data_8bit(0x2D); 
	write_data_8bit(0x01); 
	write_data_8bit(0x2C); 
	write_data_8bit(0x2D); 
	
	write_index(0xB4); //Column inversion 
	write_data_8bit(0x07); 
	
	//ST7735R Power Sequence
	write_index(0xC0); 
	write_data_8bit(0xA2); 
	write_data_8bit(0x02); 
	write_data_8bit(0x84); 
	write_index(0xC1); 
	write_data_8bit(0xC5); 

	write_index(0xC2); 
	write_data_8bit(0x0A); 
	write_data_8bit(0x00); 

	write_index(0xC3); 
	write_data_8bit(0x8A); 
	write_data_8bit(0x2A); 
	write_index(0xC4); 
	write_data_8bit(0x8A); 
	write_data_8bit(0xEE); 
	
	write_index(0xC5); //VCOM 
	write_data_8bit(0x0E); 
	
	write_index(0x36); //MX, MY, RGB mode 
	write_data_8bit(0xC0); 
	
	//ST7735R Gamma Sequence
	write_index(0xe0); 
	write_data_8bit(0x0f); 
	write_data_8bit(0x1a); 
	write_data_8bit(0x0f); 
	write_data_8bit(0x18); 
	write_data_8bit(0x2f); 
	write_data_8bit(0x28); 
	write_data_8bit(0x20); 
	write_data_8bit(0x22); 
	write_data_8bit(0x1f); 
	write_data_8bit(0x1b); 
	write_data_8bit(0x23); 
	write_data_8bit(0x37); 
	write_data_8bit(0x00); 	
	write_data_8bit(0x07); 
	write_data_8bit(0x02); 
	write_data_8bit(0x10); 

	write_index(0xe1); 
	write_data_8bit(0x0f); 
	write_data_8bit(0x1b); 
	write_data_8bit(0x0f); 
	write_data_8bit(0x17); 
	write_data_8bit(0x33); 
	write_data_8bit(0x2c); 
	write_data_8bit(0x29); 
	write_data_8bit(0x2e); 
	write_data_8bit(0x30); 
	write_data_8bit(0x30); 
	write_data_8bit(0x39); 
	write_data_8bit(0x3f); 
	write_data_8bit(0x00); 
	write_data_8bit(0x07); 
	write_data_8bit(0x03); 
	write_data_8bit(0x10);  
	
	write_index(0x2a);
	write_data_8bit(0x00);
	write_data_8bit(0x00);
	write_data_8bit(0x00);
	write_data_8bit(0x7f);

	write_index(0x2b);
	write_data_8bit(0x00);
	write_data_8bit(0x00);
	write_data_8bit(0x00);
	write_data_8bit(0x9f);
	
	write_index(0xF0); //Enable test command  
	write_data_8bit(0x01); 
	write_index(0xF6); //Disable ram power save mode 
	write_data_8bit(0x00); 
	
	write_index(0x3A); //65k mode 
	write_data_8bit(0x05); 
	
	
	write_index(0x29);//Display on	 
}

/*************************************************
��������Lcd_Clear
���ܣ�ȫ����������
��ڲ����������ɫCOLOR
����ֵ����
*************************************************/
void LCD::clear(u16 Color)               
{	
   unsigned int i,m;
   set_region(0,0,X_MAX_PIXEL-1,Y_MAX_PIXEL-1);
   write_index(0x2C);
   for(i=0;i<X_MAX_PIXEL;i++)
    for(m=0;m<Y_MAX_PIXEL;m++)
    {	
	  	write_data_16bit(Color);
    }   
}

void LCD::on()
{
led->set();
}
void LCD::off()
{
    led->reset();

}
/*************************************************
��������set_region
���ܣ�����lcd��ʾ�����ڴ�����д�������Զ�����
��ڲ�����xy�����յ�
����ֵ����
*************************************************/
void LCD::set_region(u16 x_start,u16 y_start,u16 x_end,u16 y_end)
{		
	write_index(0x2a);
	write_data_8bit(0x00);
	write_data_8bit(x_start+2);
	write_data_8bit(0x00);
	write_data_8bit(x_end+2);

	write_index(0x2b);
	write_data_8bit(0x00);
	write_data_8bit(y_start+1);
	write_data_8bit(0x00);
	write_data_8bit(y_end+1);
	
	write_index(0x2c);

}
/*************************************************
��������LCD_Set_XY
���ܣ�����lcd��ʾ��ʼ��
��ڲ�����xy����
����ֵ����
*************************************************/
void LCD::set_xy(u16 x,u16 y)
{
  	set_region(x,y,x,y);
}

	
/*************************************************
��������LCD_DrawPoint
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
inline void LCD::draw_point(u16 x,u16 y,u16 Data)
{
	set_region(x,y,x+1,y+1);
	write_data_16bit(Data);
}    

/*****************************************
 �������ܣ���TFTĳһ�����ɫ                          
 ���ڲ�����color  ����ɫֵ                                 
******************************************/
u16 LCD::read_point(u16 x,u16 y)
{
  unsigned int Data;
  set_xy(x,y);

  //Lcd_ReadData();//���������ֽ�
  //Data=Lcd_ReadData();
  write_data_8bit(Data);
  return Data;
}
