#include "OLED_i2c.h"
bool OledI2c::begin(uint32_t speed)
{
    this->speed = speed;
    i2c->begin(speed);
    i2c->take_i2c_right(speed);
    i2c->write_byte(SlaveAddr,0,0xAE);
    i2c->write_byte(SlaveAddr,0,0x00);//---set low column address
    i2c->write_byte(SlaveAddr,0,0x10);//---set high column address
    i2c->write_byte(SlaveAddr,0,0x40);//--set start line address  
    i2c->write_byte(SlaveAddr,0,0xB0);//--set page address
    i2c->write_byte(SlaveAddr,0,0x81); // contract control
    i2c->write_byte(SlaveAddr,0,0xFF);//--128   
    i2c->write_byte(SlaveAddr,0,0xA1);//set segment remap 
    i2c->write_byte(SlaveAddr,0,0xA6);//--normal / reverse
    i2c->write_byte(SlaveAddr,0,0xA8);//--set multiplex ratio(1 to 64)
    i2c->write_byte(SlaveAddr,0,0x3F);//--1/32 duty
    i2c->write_byte(SlaveAddr,0,0xC8);//Com scan direction
    i2c->write_byte(SlaveAddr,0,0xD3);//-set display offset
    i2c->write_byte(SlaveAddr,0,0x00);//
    
    i2c->write_byte(SlaveAddr,0,0xD5);//set osc division
    i2c->write_byte(SlaveAddr,0,0x80);//
    
    i2c->write_byte(SlaveAddr,0,0xD8);//set area color mode off
    i2c->write_byte(SlaveAddr,0,0x05);//
    
    i2c->write_byte(SlaveAddr,0,0xD9);//Set Pre-Charge Period
    i2c->write_byte(SlaveAddr,0,0xF1);//
    
    i2c->write_byte(SlaveAddr,0,0xDA);//set com pin configuartion
    i2c->write_byte(SlaveAddr,0,0x12);//
    
    i2c->write_byte(SlaveAddr,0,0xDB);//set Vcomh
    i2c->write_byte(SlaveAddr,0,0x30);//
    
    i2c->write_byte(SlaveAddr,0,0x8D);//set charge pump enable
    i2c->write_byte(SlaveAddr,0,0x14);//
    
    i2c->write_byte(SlaveAddr,0,0xAF);//--turn on oled panel
    i2c->release_i2c_right();
}
void OledI2c::write_cmd(uint8_t cmd)
{
    i2c->take_i2c_right(speed);
    i2c->write_byte(SlaveAddr,0,cmd);//--turn on oled panel
    i2c->release_i2c_right();

}
void OledI2c::write_data(uint8_t data)
{
    i2c->take_i2c_right(speed);
    i2c->write_byte(SlaveAddr,0x40,data);//--turn on oled panel
    i2c->release_i2c_right();

}
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void OledI2c::clear(void)  
{  
    u8 i,n;		    
    for(i=0;i<8;i++)  
    {  
        write_cmd(0xb0+i);//设置页地址（0~7）
        write_cmd(0X00);//设置显示位置—列低地址
        write_cmd(0X10);//设置显示位置—列高地址   
        for(n=0;n<128;n++)
            write_data(0X00); 
    } //更新显示
}
void OledI2c::on(void)  
{  
    u8 i,n;		    
    for(i=0;i<8;i++)  
    {  
        
        write_cmd(0xb0+i);
        write_cmd(0X00);
        write_cmd(0X10);
        for(n=0;n<128;n++)
            write_data(0xff);
        for(n=0;n<128;n++)
            write_data(0x01);
    } //更新显示
}

//开启OLED显示    
void OledI2c::display_on(void)
{
    write_cmd(0X8D);
    write_cmd(0X14);
    write_cmd(0XAF);
}
//关闭OLED显示     
void OledI2c::display_off(void)
{
    write_cmd(0X8D);
    write_cmd(0X10);
    write_cmd(0XAE);
}		   			 

void OledI2c::set_pos(unsigned char x, unsigned char y) 
{
    write_cmd(0xb0+y);
    write_cmd(((x&0xf0)>>4)|0x10);
    write_cmd(x&0x0f); 
}   
//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OledI2c::show_char(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
    unsigned char c=0,i=0;	
    c=chr-' ';//得到偏移后的值			
    if(x>Max_Column-1){x=0;y=y+2;}
    if(Char_Size ==16)
    {
        set_pos(x,y);	
        for(i=0;i<8;i++)
            write_data(font8x16[c*16 + i]); 

        set_pos(x,y+1);
        for(i=0;i<8;i++)
            write_data(font8x16[c*16 + i + 8]); 
    }
    else 
    {	
        set_pos(x,y);
        for(i=0;i<6;i++)
            write_data(font6x8[c][i]); 
    }
}
//显示一个字符号串
void OledI2c::show_string(u8 x,u8 y, char *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		
        show_char(x,y,chr[j],Char_Size);
        x+=8;
		if(x>120)
        {
            x=0;
            y+=2;
        }
		j++;
	}
}

//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OledI2c::show_num(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(int)(num/pow((float)10,(float)len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				show_char(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	show_char(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 

//显示汉字
void OledI2c::show_chinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	set_pos(x,y);	
    for(t=0;t<16;t++)
    {
        write_data(Hzk[2*no][t]);
        adder+=1;
     }	
    set_pos(x,y+1);	
    for(t=0;t<16;t++)
    {	
        write_data(Hzk[2*no + 1][t]);
        adder+=1;
    }					
}

/********************************************
// fill_Picture
********************************************/
void OledI2c::fill_picture(unsigned char fill_Data)
{
	unsigned char m,n;
	for(m=0;m<8;m++)
	{
        write_cmd(0xb0+m);//设置页地址（0~7）
        write_cmd(0X00);//设置显示位置—列低地址
        write_cmd(0X10);//设置显示位置—列高地址   

		for(n=0;n<128;n++)
        {
            write_data(fill_Data);
        }
	}
}
/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OledI2c::draw_bmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
        set_pos(x0,y);
        for(x=x0;x<x1;x++)
	    {      
            write_data(BMP[j++]);
	    }
	}
} 



