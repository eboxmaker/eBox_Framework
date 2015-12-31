
#include "ebox.h"


class LCD
{
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
        void clear(u16 Color);
        void on();
        void off();
        void set_xy(u16 x,u16 y);
        inline void draw_point(u16 x,u16 y,u16 Data);
        u16 read_point(u16 x,u16 y);

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
