/**
  ******************************************************************************
  * @file    spi.h
  * @author  shentq
  * @version V2.1
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

#include "ebox_core.h"
#include "spi.h"
#include "mcu.h"
#include "dma.h"



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

    virtual int8_t  write_buf(uint8_t *data, uint16_t len);
    virtual int8_t  read_buf(uint8_t *recv_data, uint16_t len);


public:
    virtual int8_t  take(Config_t *newConfig);
    virtual int8_t  release(void);

    virtual int8_t      dma_write(uint8_t data);
    virtual uint8_t     dma_read();
    virtual uint16_t    dma_write_buf(uint8_t *data, uint16_t len);
    virtual uint16_t    dma_read_buf(uint8_t *recv_data, uint16_t len);
    virtual void        dma_wait();

private:
    uint8_t tx_buffer[1];
    uint8_t rx_buffer[1];

    void dma_config( void );

private:
    SPI_TypeDef *_spi;
    Gpio        *_sck;
    Gpio        *_miso;
    Gpio        *_mosi;

    uint8_t     _busy;
    DMA_InitTypeDef dmaRxCfg;
    DMA_InitTypeDef dmaTxCfg;
    Dma *dmaTx;
    Dma *dmaRx;

};

#endif
