/**
  ******************************************************************************
  * @file    spi.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __SPI_H
#define __SPI_H

#include "ebox_common.h"
//spi��ع����������//////////////////////////////////
#define SPI_MODE0 0x02
#define SPI_MODE1 0x00
#define SPI_MODE2 0x03
#define SPI_MODE3 0x01

#define SPI_CLOCK_DIV2     SPI_BaudRatePrescaler_2
#define SPI_CLOCK_DIV4     SPI_BaudRatePrescaler_4
#define SPI_CLOCK_DIV8     SPI_BaudRatePrescaler_8
#define SPI_CLOCK_DIV16    SPI_BaudRatePrescaler_16
#define SPI_CLOCK_DIV32    SPI_BaudRatePrescaler_32
#define SPI_CLOCK_DIV64    SPI_BaudRatePrescaler_64
#define SPI_CLOCK_DIV128   SPI_BaudRatePrescaler_128
#define SPI_CLOCK_DIV256   SPI_BaudRatePrescaler_256

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

    uint8_t transfer(uint8_t data);


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
};

#endif
