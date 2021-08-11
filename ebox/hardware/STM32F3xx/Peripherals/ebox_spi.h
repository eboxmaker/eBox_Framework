#ifndef __EBOX_SPI_H
#define __EBOX_SPI_H
#include "ebox_core.h"
#include "mcu.h"
#include "spi.h"
//spi相关公共宏和声明//////////////////////////////////



/*
	1.目前只测试了SPI1、SPI2，spi3望网友测试
	2.该spi功能强大，总线支持同时挂载不同MODE ,SPEED,bit_oder的设备
	3.每一个spi设备应有一个自己的SPI_CONFIG的配置，以支持该设备的的读写，
		在读写前需要获得SPI的控制权，如果获取不到则一直等待！主要是为了兼容操作系统，
		如果不使用操作系统也必须加上获得控制权的代码，在是用完SPI后一定要释放SPI总线，
		如果不释放总线会导致别的SPI设备一直处于等待的状态
*/
//默认配置 空，只依靠结构体SPICONFIG来初始化
class mcuSpi: public Spi
{
public:
    mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

    virtual void    begin (Spi::Config_t *spi_config);
    virtual void    config(Spi::Config_t *spi_config);
    virtual uint8_t read_config(void);

    virtual uint8_t transfer(uint8_t data);

    virtual int8_t  write(uint8_t data);
    virtual uint8_t read();
    virtual int8_t  read(uint8_t  *recv_data);

    virtual int8_t  write_buf(uint8_t *data, uint16_t len);
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t len);
public:
    virtual int8_t  take(Spi::Config_t *spi_config);
    virtual int8_t  release(void);
private:
    SPI_TypeDef *_spi;
    Gpio        *_sck;
    Gpio        *_miso;
    Gpio        *_mosi;

    uint8_t     _busy;

};

#endif
