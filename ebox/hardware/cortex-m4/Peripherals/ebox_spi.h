#ifndef __EBOX_SPI_H
#define __EBOX_SPI_H
#include "ebox_common.h"
//spi��ع����������//////////////////////////////////



#define SPI_BITODER_MSB		SPI_FirstBit_MSB
#define SPI_BITODER_LSB		SPI_FirstBit_LSB

typedef struct
{
    uint8_t     dev_num;
    uint8_t     mode;
    uint16_t    prescaler;
    uint16_t    bit_order;
} SPI_CONFIG_TYPE;


/*
	1.Ŀǰֻ������SPI1��SPI2��spi3�����Ѳ���
	2.��spi����ǿ������֧��ͬʱ���ز�ͬMODE ,SPEED,bit_oder���豸
	3.ÿһ��spi�豸Ӧ��һ���Լ���SPI_CONFIG�����ã���֧�ָ��豸�ĵĶ�д��
		�ڶ�дǰ��Ҫ���SPI�Ŀ���Ȩ�������ȡ������һֱ�ȴ�����Ҫ��Ϊ�˼��ݲ���ϵͳ��
		�����ʹ�ò���ϵͳҲ������ϻ�ÿ���Ȩ�Ĵ��룬��������SPI��һ��Ҫ�ͷ�SPI���ߣ�
		������ͷ����߻ᵼ�±��SPI�豸һֱ���ڵȴ���״̬
*/
//Ĭ������ �գ�ֻ�����ṹ��SPICONFIG����ʼ��
class	Spi
{
public:
    Spi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

    void    begin (SPI_CONFIG_TYPE *spi_config);
    void    config(SPI_CONFIG_TYPE *spi_config);
    uint8_t read_config(void);

    uint8_t transfer(uint8_t data);

    int8_t  write(uint8_t data);
    int8_t  write(uint8_t *data, uint16_t data_length);

    uint8_t read();
    int8_t  read(uint8_t  *recv_data);
    int8_t  read(uint8_t *recv_data, uint16_t data_length);
public:
    int8_t take_spi_right(SPI_CONFIG_TYPE *spi_config);
    int8_t release_spi_right(void);
private:
    uint8_t     current_dev_num;
    SPI_TypeDef *spi;
    uint8_t     busy;

		Gpio *sck;
		Gpio *miso;
		Gpio *mosi;
		
};
/*
	ע�⣺1.�����SPI_CLOCK_DIV����delay_us��ʱ�������ơ����в�׼����Ӳ��SPI�����ܶ�
				2.speed����ֻ��ΪSPI_CLOCK_DIVx��������Ǵ�ֵ����ὫSPI_CLOCK_DIV��ֱֵ�Ӵ��ݸ�delay_us.��delay_us(SPI_CONFIG_TYPE->prescaler);
				3.���ڵ���I2C�豸����ʹ��SPI_CLOCK_DIV256��
				4.�����ӿں�Ӳ��SPI��ȫһ�����Ի����滻��
*/
class SoftSpi
{
public:
    SoftSpi(Gpio *sck, Gpio *miso, Gpio *mosi);

    void    begin(SPI_CONFIG_TYPE *spi_config);
    void    config(SPI_CONFIG_TYPE *spi_config);
    uint8_t read_config(void);

    int8_t  write(uint8_t data);
    int8_t  write(uint8_t *data, uint16_t data_length);

    uint8_t read();
    int8_t  read(uint8_t *data);
    int8_t  read(uint8_t *rcvdata, uint16_t data_length);
public:
    int8_t take_spi_right(SPI_CONFIG_TYPE *spi_config);
    int8_t release_spi_right(void);

private:
    Gpio    *sck_pin;
    Gpio    *mosi_pin;
    Gpio    *miso_pin;

    uint8_t mode;
    uint8_t bit_order;
    uint8_t spi_delay;

    uint8_t current_dev_num;
    uint8_t busy;

    uint8_t transfer0(uint8_t data);
    uint8_t transfer1(uint8_t data);
    uint8_t transfer2(uint8_t data);
    uint8_t transfer3(uint8_t data);
    uint8_t transfer(uint8_t data);
};

#endif
