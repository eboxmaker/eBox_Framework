#ifndef __EBOX_PORT_SPI_H
#define __EBOX_PORT_SPI_H


class eBoxSpi
{
    public:
    eBoxSpi(){};

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
};

#endif
