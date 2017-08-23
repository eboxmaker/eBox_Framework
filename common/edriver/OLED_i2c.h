#ifndef __OLED_IIC_H
#define __OLED_IIC_H

#include "ebox.h"
#include "font.h"
#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
#define SlaveAddr  0x78


#define OLED_MODE 0
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 


class OledI2c
{
    public:
        OledI2c(SoftI2c *i2c)
        {
            this->i2c = i2c;
            speed = 0;
        }
        bool begin(uint32_t speed);
        
        void clear(void);
        void on(void);  
        void display_on(void);
        void display_off(void);
        
        void show_char(u8 x,u8 y,u8 chr,u8 Char_Size);
        void show_string(u8 x,u8 y,char  *chr,u8 Char_Size);
        void show_num(u8 x,u8 y,u32 num,u8 len,u8 size2);
        void show_chinese(u8 x,u8 y,u8 no);

        
        void fill_picture(unsigned char fill_Data);
        void draw_bmp(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);

    private:
        SoftI2c *i2c;
        uint32_t speed;
        void write_cmd(uint8_t cmd);
        void write_data(uint8_t data);
        void set_pos(unsigned char x, unsigned char y) ;

};

#endif
