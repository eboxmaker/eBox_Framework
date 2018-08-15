#ifndef __EBOX_PORT_SPI_H
#define __EBOX_PORT_SPI_H

#include "ebox_type.h"


//spi相关公共宏和声明//////////////////////////////////
#define SPI_MODE0 0
#define SPI_MODE1 1
#define SPI_MODE2 2
#define SPI_MODE3 3

#define SPI_CLOCK_DIV2     0
#define SPI_CLOCK_DIV4     1
#define SPI_CLOCK_DIV8     2
#define SPI_CLOCK_DIV16    3
#define SPI_CLOCK_DIV32    4
#define SPI_CLOCK_DIV64    5
#define SPI_CLOCK_DIV128   6
#define SPI_CLOCK_DIV256   7


typedef struct
{
    uint8_t     dev_num;
    uint8_t     mode;
    uint8_t     prescaler;
    uint8_t     bit_order;
} SpiConfig_t;
class Spi
{
    private:
        uint8_t dev_num_acc;
    protected:
        uint8_t current_dev_num;

    public:
        Spi():dev_num_acc(0),current_dev_num(0){ };

        virtual void    begin (SpiConfig_t *spi_config)  = 0;
        virtual void    config(SpiConfig_t *spi_config)  = 0;
        virtual uint8_t read_config(void)  = 0;

        virtual uint8_t transfer(uint8_t data)  = 0;

        virtual int8_t  write(uint8_t data)  = 0;
        virtual int8_t  write(uint8_t *data, uint16_t data_length)  = 0;

        virtual uint8_t read()  = 0;
        virtual int8_t  read(uint8_t  *recv_data)  = 0;
        virtual int8_t  read(uint8_t *recv_data, uint16_t data_length)  = 0;

        virtual int8_t take_spi_right(SpiConfig_t *spi_config)  = 0;
        virtual int8_t release_spi_right(void)  = 0;
        uint8_t get_new_dev_num()
        {
            dev_num_acc++;
            return dev_num_acc;
        }
};

#endif
