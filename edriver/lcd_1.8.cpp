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
void LCD::soft_reset()
{
	write_index(0x01);//soft reset
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
	
    MADCTL = 0XC0;
	write_index(0x36); //MX, MY, RGB mode 
	write_data_8bit(MADCTL); 
	
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

void LCD::on()
{
	write_index(0x29);//Display on	 
}
void LCD::off()
{
	write_index(0x28);//Display off	 

}
void LCD::column_order(u8 order)
{
	write_index(0x36); //MX, MY, RGB mode 
    if(order == 1){
        MADCTL |= 1 << 6;
    }
    else
        MADCTL &= ~(1 << 6);
    write_data_8bit(MADCTL); 

}
void LCD::row_order(u8 order)
{
	write_index(0x36); //MX, MY, RGB mode 
    if(order == 1){
        MADCTL |= 1 << 7;
    }
    else
        MADCTL &= ~(1 << 7);
    write_data_8bit(MADCTL); 

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
   for(i = 0; i < X_MAX_PIXEL * Y_MAX_PIXEL; i++){
	  	write_data_16bit(Color);
    }   
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



/*************************************************
��������draw_h_line
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void LCD::draw_h_line(int x0, int y,  int x1)
{
	set_region(x0,y,x1,y);
    for (; x0 <= x1; x0++) {
        write_data_16bit(front_color);
    }
}    

/*************************************************
��������draw_v_line
���ܣ���һ����
��ڲ�������
����ֵ����
*************************************************/
void LCD::draw_v_line(int x, int y0,  int y1)
{
	set_region(x,y0,x,y1);
    
    for (; y0 <= y1; y0++) {
        write_data_16bit(front_color);
    }
}    
void LCD::fill_rect(int x0, int y0,  int x1, int y1)
{
    u16 i = 0;
    u8 dx,dy;
    dx = x1 - x0 + 1;
    dy = y1 - y0 + 1;
    
	set_region(x0,y0,x1,y1);
    for (; i <= dx * dy; i++) {
        write_data_16bit(front_color);
    }
}
void LCD::draw_circle(u16 x,u16 y,u16 r) 
{//Bresenham�㷨 
    unsigned short  a,b; 
    int c; 
    a=0; 
    b=r; 
    c=3-2*r; 
    while (a<b) 
    { 
        draw_point(x+a,y+b,front_color);     //        7 
        draw_point(x-a,y+b,front_color);     //        6 
        draw_point(x+a,y-b,front_color);     //        2 
        draw_point(x-a,y-b,front_color);     //        3 
        draw_point(x+b,y+a,front_color);     //        8 
        draw_point(x-b,y+a,front_color);     //        5 
        draw_point(x+b,y-a,front_color);     //        1 
        draw_point(x-b,y-a,front_color);     //        4 

        if(c<0) c=c+4*a+6; 
        else 
        { 
            c=c+4*(a-b)+10; 
            b-=1; 
        } 
       a+=1; 
    } 
    if (a==b) 
    { 
        draw_point(x+a,y+b,front_color); 
        draw_point(x+a,y+b,front_color); 
        draw_point(x+a,y-b,front_color); 
        draw_point(x-a,y-b,front_color); 
        draw_point(x+b,y+a,front_color); 
        draw_point(x-b,y+a,front_color); 
        draw_point(x+b,y-a,front_color); 
        draw_point(x-b,y-a,front_color); 
    } 
	
}
//���ߺ�����ʹ��Bresenham �����㷨
void LCD::draw_line(u16 x0, u16 y0,u16 x1, u16 y1)   
{
int dx,             // difference in x's
    dy,             // difference in y's
    dx2,            // dx,dy * 2
    dy2, 
    x_inc,          // amount in pixel space to move during drawing
    y_inc,          // amount in pixel space to move during drawing
    error,          // the discriminant i.e. error i.e. decision variable
    index;          // used for looping	


	set_xy(x0,y0);
	dx = x1-x0;//����x����
	dy = y1-y0;//����y����

	if (dx>=0)
	{
		x_inc = 1;
	}
	else
	{
		x_inc = -1;
		dx    = -dx;  
	} 
	
	if (dy>=0)
	{
		y_inc = 1;
	} 
	else
	{
		y_inc = -1;
		dy    = -dy; 
	} 

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy)//x�������y���룬��ôÿ��x����ֻ��һ���㣬ÿ��y���������ɸ���
	{//���ߵĵ�������x���룬��x���������
		// initialize error term
		error = dy2 - dx; 

		// draw the line
		for (index=0; index <= dx; index++)//Ҫ���ĵ������ᳬ��x����
		{
			//����
			draw_point(x0,y0,front_color);
			
			// test if error has overflowed
			if (error >= 0) //�Ƿ���Ҫ����y����ֵ
			{
				error-=dx2;

				// move to next line
				y0+=y_inc;//����y����ֵ
			} // end if error overflowed

			// adjust the error term
			error+=dy2;

			// move to the next pixel
			x0+=x_inc;//x����ֵÿ�λ���󶼵���1
		} // end for
	} // end if |slope| <= 1
	else//y�����x�ᣬ��ÿ��y����ֻ��һ���㣬x�����ɸ���
	{//��y��Ϊ��������
		// initialize error term
		error = dx2 - dy; 

		// draw the line
		for (index=0; index <= dy; index++)
		{
			// set the pixel
			draw_point(x0,y0,front_color);

			// test if error overflowed
			if (error >= 0)
			{
				error-=dy2;

				// move to next line
				x0+=x_inc;
			} // end if error overflowed

			// adjust the error term
			error+=dx2;

			// move to the next pixel
			y0+=y_inc;
		} // end for
	} // end else |slope| > 1
}
void LCD::disp_char6x8(uint8_t row,uint8_t col,u8 ch)
{
    u8 x,y,i=0,j=0;
    x = (X_MAX_PIXEL / 6) * col; 
    y = (Y_MAX_PIXEL / 8) * row;
    if(ch >= 0x20)ch -= 0x20;
	set_region(x,y,x+6,y+8);
    for (i = 0; i <= 6; i++) {
        for (j = 0; j <= 8; j++) {
            if(font6x8[ch][i] & (0x80 >> j))
                write_data_16bit(front_color);
            else
                write_data_16bit(back_color);
                
        }
    }
   
}
void LCD::disp_char8x16(uint8_t row,uint8_t col,u8 ch)
{
    u8 x,y,i=0,j=0;
    x = (8) * col; 
    y = (16) * row;
    if(ch >= 0x20)ch -= 0x20;
	set_region(x,y,x+8,y+16);
    for (i = 0; i <= 16; i++) {
        for (j = 0; j <= 8; j++) {
            if(asc16[ch*16 + i] & (0x80 >> j))
                write_data_16bit(front_color);
            else
                write_data_16bit(back_color);
                
        }
    }   
}
void LCD::printf(uint8_t row,uint8_t col,const char *fmt,...)
{
	char buf[30];
	u8 i = 0;
	va_list va_params;   
	va_start(va_params,fmt);   
	vsprintf(buf,fmt,va_params);   
	va_end(va_params); 
	while(buf[i] != '\0')
	{
        if(col*8 > X_MAX_PIXEL - 7){
            row++;
            col = 0;
        }
	  disp_char8x16(row,col++,buf[i++]);
	}

}

