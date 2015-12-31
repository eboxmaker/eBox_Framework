
#include "ebox.h"


class LCD
{
    public:
        uint8_t MADCTL;
        u16 front_color;
        u16 back_color;
        u8 row;
        u8 col;
    public:
        LCD(GPIO *cs,GPIO *led,GPIO *rs,GPIO *rst,SPI *spi)
        {
            this->cs = cs;
            this->led = led;
            this->rs = rs;
            this->rst = rst;
            this->spi = spi;
        }
        void begin(u8 dev_num);
        void soft_reset();
        void clear(u16 Color);
        void on();
        void off();
        void column_order(u8 order);
        void row_order(u8 order);

        void set_xy(u16 x,u16 y);
        inline void draw_point(u16 x,u16 y,u16 Data);
        u16 read_point(u16 x,u16 y);
        
        void draw_h_line(int x0, int y ,  int x1);
        void draw_v_line(int x , int y0,  int y1);
        void fill_rect  (int x0, int y0,  int x1, int y1);
        void draw_circle(u16 x,  u16 y,   u16 r);
        void draw_line(u16 x0, u16 y0,u16 x1, u16 y1);
        
        void disp_char6x8(uint8_t row,uint8_t col,u8 ch);
        void disp_char8x16(uint8_t row,uint8_t col,u8 ch);
        void printf(uint8_t row,uint8_t col,const char *fmt,...);
    private:
        GPIO *cs;
        GPIO *led;
        GPIO *rs;
        GPIO *rst;
        SPI  *spi;
        SPI_CONFIG_TYPE spi_dev_lcd;
    
        void reset(void);       
        void init(void);
        void spi_write_data(u8 Data);
        void write_index(u8 Index);
        void write_data_8bit(u8 Data);
        void write_data_16bit(u16 Data);
        void write_reg(u8 Index,u8 Data);
        void set_region(u16 x_start,u16 y_start,u16 x_end,u16 y_end);
       
};
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//»ÒÉ«0 3165 00110 001011 00101
#define GRAY1   0x8410      	//»ÒÉ«1      00000 000000 00000
#define GRAY2   0x4208      	//»ÒÉ«2  1111111111011111
