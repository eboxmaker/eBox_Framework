#ifndef __EBOX_PORT_SPI_H
#define __EBOX_PORT_SPI_H

#include "ebox_type.h"


//spi相关公共宏和声明//////////////////////////////////






class Spi
{
    
public:
        
    typedef enum 
    {
        MODE0 = 0,
        MODE1 = 1,
        MODE2 = 2,
        MODE3 = 3
    }Mode_t;
    
    typedef enum 
    {
        DIV2     = 0,
        DIV4     = 1,
        DIV8     = 2,
        DIV16    = 3,
        DIV32    = 4,
        DIV64    = 5,
        DIV128   = 6,
        DIV256   = 7
    }Clock_t;
    
    typedef enum 
    {
        LSB = 0,
        MSB = 1
    }BitOrder_t;
       
    typedef struct
    {
        uint8_t     dev_num;
        Mode_t      mode;
        Clock_t     prescaler;
        BitOrder_t  bit_order;
    } Config_t;

protected:
    uint8_t current_dev_num;        // 设备ID，用来区分spi总线上挂载的不同设备，建议使用cs pinid 作为设备id
public:

    virtual void    begin (Config_t *cfg)  = 0;
    virtual void    config(Config_t *cfg)  = 0;
    virtual uint8_t read_config(void)  = 0;

    virtual uint8_t transfer(uint8_t data)  = 0;
    // 单字节读写
    virtual int8_t  write(uint8_t data)  = 0;
    virtual uint8_t read()  = 0;
    // buf读写
    virtual int8_t  write_buf(uint8_t *data, uint16_t data_length)  = 0;
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t data_length)  = 0;


    virtual int8_t      dma_write(uint8_t data){ return 0;};
    virtual uint8_t     dma_read(){ return 0;};
    virtual uint16_t    dma_write_buf(uint8_t *data, uint16_t len){ return 0;};
    virtual uint16_t    dma_read_buf(uint8_t *recv_data, uint16_t len){ return 0;};
    virtual void        dma_wait(){};
        
    virtual int8_t  take(Config_t *spi_config)  = 0;
    virtual int8_t  release(void)  = 0;


};

#endif
