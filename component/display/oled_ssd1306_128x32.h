#ifndef __oled_ssd1306_128x32_h
#define __oled_ssd1306_128x32_h

#include "ebox_core.h"
#include "i2c.h"
#include "spi.h"
#include "hmi.h"
//#include "font.h"

#define OLED_CMD  0	//Ð´ÃüÁî
#define OLED_DATA 1	//Ð´Êý¾Ý
#define SlaveAddr  0x78


#define OLED_MODE 0
#define XLevelL		0x00
#define XLevelH		0x10
#define SSD1306_LCDWIDTH	128
#define SSD1306_LCDHEIGHT		32
#define SSD1306_BITS_PER_PIXEL  1
#define SSD1306_128_32_1

#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	32


class OledSSD1306_128x32:public HMI
{
public:
    OledSSD1306_128x32(I2c *i2c)
    {
        this->i2c = i2c;
    }
    OledSSD1306_128x32(Spi *spi,Gpio *cs,Gpio *reset)
    {
        this->spi = spi;
        this->reset = reset;
    }
    bool begin(I2c::Speed_t speed = I2c::K400);
    bool begin(Spi::Config_t *cfg);

    void clear(void);
    void on(void);
    void display_on(void);
    void display_off(void);
    void draw_pixel(int16_t x, int16_t y,uint32_t color);
    void fill_screen(uint32_t color);
    void flush();

    void show_char(uint8_t x, uint8_t y, uint8_t chr, uint8_t Char_Size);
    void show_string(uint8_t x, uint8_t y, char  *chr, uint8_t Char_Size);
    void show_num(uint8_t x, uint8_t y, uint32_t num, uint8_t len, uint8_t size2);
    void show_chinese(uint8_t x, uint8_t y, uint8_t no);


    void fill_picture(unsigned char fill_Data);
    void draw_bmp(unsigned char x0, unsigned char y0, unsigned char BMP[], unsigned char width, unsigned char hight );

private:
    I2c *i2c;
    Spi *spi;
    Gpio *reset;
    uint8_t WriteEscSeqP(const uint8_t *esc_seq);
    uint8_t write_byte(uint8_t value);
    void write_cmd(uint8_t cmd);
    void write_data(uint8_t data);
    void set_pos(unsigned char x, unsigned char y) ;


    int16_t width;
    int16_t hight;
    uint16_t buffer_size;
};

#endif
