#ifndef __EBOX_SLAVE_SPI_H
#define __EBOX_SLAVE_SPI_H

#include "ebox_core.h"
#include "ringbuf.h"
class	SlaveSpi
{
public:
    SlaveSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

     void    begin ();
     void    config(SpiConfig_t *spi_config);
    void    interrupt(FunctionalState enable,uint8_t preemption_priority = 0, uint8_t sub_priority = 0);

    void    enable_dma_send(uint8_t *buffer,uint16_t len);
    void    enable_dma_recv(uint8_t *buffer,uint16_t len);
    void    enable_rx_irq();
    void    disable_rx_irq();

    uint8_t transfer(uint8_t data);
    
    uint8_t read_buf_pool[100];
    uint8_t write_buf_pool[100];
    RINGBUF read_buf;
    RINGBUF write_buf;
//    virtual uint8_t read_config(void);

//    virtual uint8_t transfer(uint8_t data);

//    virtual int8_t  write(uint8_t data);
//    virtual int8_t  write(uint8_t *data, uint16_t data_length);

//    virtual uint8_t read();
//    virtual int8_t  read(uint8_t  *recv_data);
//    virtual int8_t  read(uint8_t *recv_data, uint16_t data_length);
//public:
//    virtual int8_t take_spi_right(SpiConfig_t *spi_config);
//    virtual int8_t release_spi_right(void);
    uint8_t dma_buf[10];
private:
    SPI_TypeDef *spi;
    uint8_t     busy;
    DMA_Channel_TypeDef *_DMA1_Channelx;

};

extern SlaveSpi slave_spi1;
#endif


