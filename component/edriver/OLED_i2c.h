#ifndef __OLED_IIC_H
#define __OLED_IIC_H

#include "ebox_core.h"
#include "i2c.h"
#include "hmi.h"
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
    OledI2c(I2c *i2c)
    {
        this->i2c = i2c;
    }
    bool begin(I2c::Speed_t speed = I2c::K100);

    void clear(void);
    void on(void);
    void display_on(void);
    void display_off(void);

    void show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
    void show_string(uint8_t x, uint8_t y, char  *chr, uint8_t Char_Size);
    void show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
    void show_chinese(uint8_t x, uint8_t y, uint8_t no);


    void fill_picture(unsigned char fill_Data);
    void draw_bmp(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[]);

private:
    I2c *i2c;
    void write_cmd(uint8_t cmd);
    void write_data(uint8_t data);
    void set_pos(unsigned char x, unsigned char y) ;

};

#endif
