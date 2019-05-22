#ifndef __EBOX_SPI_H
#define __EBOX_SPI_H
#include "ebox_core.h"
#include "mcu.h"
//spi��ع����������//////////////////////////////////



/*
	1.Ŀǰֻ������SPI1��SPI2��spi3�����Ѳ���
	2.��spi����ǿ������֧��ͬʱ���ز�ͬMODE ,SPEED,bit_oder���豸
	3.ÿһ��spi�豸Ӧ��һ���Լ���SPI_CONFIG�����ã���֧�ָ��豸�ĵĶ�д��
		�ڶ�дǰ��Ҫ���SPI�Ŀ���Ȩ�������ȡ������һֱ�ȴ�����Ҫ��Ϊ�˼��ݲ���ϵͳ��
		�����ʹ�ò���ϵͳҲ������ϻ�ÿ���Ȩ�Ĵ��룬��������SPI��һ��Ҫ�ͷ�SPI���ߣ�
		������ͷ����߻ᵼ�±��SPI�豸һֱ���ڵȴ���״̬
*/
//Ĭ������ �գ�ֻ�����ṹ��SPICONFIG����ʼ��
class mcuSpi: public Spi
{
public:
    mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

    virtual void    begin (Config_t *newConfig);
    virtual void    config(Config_t *newConfig);
    virtual uint8_t read_config(void);

    virtual uint8_t transfer(uint8_t data);

    virtual int8_t  write(uint8_t data);
    virtual uint8_t read();
    virtual int8_t  read(uint8_t  *recv_data);

    virtual int8_t  write_buf(uint8_t *data, uint16_t len);
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t len);
public:
    virtual int8_t  take(Config_t *newConfig);
    virtual int8_t  release(void);
private:
    SPI_TypeDef *_spi;
    Gpio        *_sck;
    Gpio        *_miso;
    Gpio        *_mosi;

    uint8_t     _busy;

};



/*
	ע�⣺1.�����SPI_CLOCK_DIV����delay_us��ʱ�������ơ����в�׼����Ӳ��SPI�����ܶ�
				2.speed����ֻ��ΪSPI_CLOCK_DIVx��������Ǵ�ֵ����ὫSPI_CLOCK_DIV��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(SPI_CONFIG_TYPE->prescaler);
				3.���ڵ���I2C�豸����ʹ��SPI_CLOCK_DIV256��
				4.�����ӿں�Ӳ��SPI��ȫһ�����Ի����滻��
*/
class SoftSpi: public Spi
{
public:
    SoftSpi(Gpio *sck, Gpio *miso, Gpio *mosi);

    virtual void    begin (Config_t *spi_config);
    virtual void    config(Config_t *spi_config);
    virtual uint8_t read_config(void);

    virtual uint8_t transfer(uint8_t data);

    virtual int8_t  write(uint8_t data);
    virtual uint8_t read();
    virtual int8_t  read(uint8_t  *recv_data);

    virtual int8_t  write_buf(uint8_t *data, uint16_t len);
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t len);

public:
    virtual int8_t take(Config_t *spi_config);
    virtual int8_t release(void);

private:
    Gpio    *_sck;
    Gpio    *_miso;
    Gpio    *_mosi;
    uint8_t _busy;

    Spi::Mode_t mode;
    Spi::BitOrder_t bit_order;
    uint8_t spi_delay;


    uint8_t transfer0(uint8_t data);
    uint8_t transfer1(uint8_t data);
    uint8_t transfer2(uint8_t data);
    uint8_t transfer3(uint8_t data);
};


#endif
