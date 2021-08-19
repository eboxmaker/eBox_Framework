
/*
file   : _spi.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "ebox_mcu_spi.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define EBOX_DEBUG_MCUSPI_ENABLE       true
#define EBOX_DEBUG_MCUSPI_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_MCUSPI_ENABLE
#define mcuSpiDebug(...)  ebox_printf("[mcuSPI DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define mcuSpiDebug(...)
#endif

#if EBOX_DEBUG_MCUSPI_ENABLE_ERR
#define mcuSpiDebugErr(fmt, ...)  ebox_printf("[mcuSPI err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define mcuSpiDebugErr(fmt, ...)
#endif

mcuSpi::mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    _busy = 0;
    _spi = SPIx;
    _sck = sck;
    _miso = miso;
    _mosi = mosi;

}

void mcuSpi::begin(Config_t *newConfig)
{

    _sck->mode(AF_PP);
    _miso->mode(AF_PP);
    _mosi->mode(AF_PP);

    rcc_clock_cmd((uint32_t)_spi, ENABLE);
    config(newConfig);
}
void mcuSpi::config(Config_t *newConfig)
{
    LL_SPI_InitTypeDef SPI_InitStructure;
    
    current_dev_num = newConfig->dev_num;

    LL_SPI_Disable(_spi);
    LL_SPI_DeInit(_spi);
    SPI_InitStructure.TransferDirection = LL_SPI_FULL_DUPLEX;//ȫ˫��
    SPI_InitStructure.DataWidth = LL_SPI_DATAWIDTH_8BIT;//8λ����ģʽ
    SPI_InitStructure.NSS = LL_SPI_NSS_SOFT;//NSS�������
    SPI_InitStructure.CRCPoly = 7;//CRC����ʽ
    SPI_InitStructure.Mode = LL_SPI_MODE_MASTER;//����ģʽ
    if(newConfig->mode == MODE0)
    {
        SPI_InitStructure.ClockPhase = LL_SPI_PHASE_1EDGE;
        SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_LOW;
    }
    else if(newConfig->mode == MODE1)
    {
        SPI_InitStructure.ClockPhase = LL_SPI_PHASE_2EDGE;
        SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_LOW;
    }
    else if(newConfig->mode == MODE2)
    {
        SPI_InitStructure.ClockPhase = LL_SPI_PHASE_1EDGE;
        SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_HIGH;
    }
    else if(newConfig->mode == MODE3)
    {
        SPI_InitStructure.ClockPhase = LL_SPI_PHASE_2EDGE;
        SPI_InitStructure.ClockPolarity = LL_SPI_POLARITY_HIGH;
    }
    switch(newConfig->prescaler)
    {
    case DIV2:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV2;
        break;
    case DIV4:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV4;
        break;
    case DIV8:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8;
        break;
    case DIV16:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV16;
        break;
    case DIV32:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV32;
        break;
    case DIV64:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV64;
        break;
    case DIV128:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV128;
        break;
    case DIV256:
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
        break;
    default :
        SPI_InitStructure.BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV256;
        break;

    }

    switch(newConfig->bit_order)
    {
    case MSB:
        SPI_InitStructure.BitOrder = LL_SPI_MSB_FIRST;
        break;
    case LSB:
        SPI_InitStructure.BitOrder = LL_SPI_LSB_FIRST;
        break;
    default :
        SPI_InitStructure.BitOrder = LL_SPI_MSB_FIRST;
        break;
    }
    LL_SPI_Init(_spi, &SPI_InitStructure);
    LL_SPI_Enable(_spi);
}

uint8_t mcuSpi::read_config(void)
{
    return current_dev_num;
}


/**
  *@brief    transfer
  *@param    data Ҫ���͵�����
  *@retval   uint8_t ���ܵ�������
  */
uint8_t mcuSpi::transfer(uint8_t data)
{
    while (LL_SPI_IsActiveFlag_TXE(_spi) == RESET)
        ;
    _spi->DR = data;
    while (LL_SPI_IsActiveFlag_RXNE(_spi) == RESET)
        ;
    return _spi->DR;
}

/**
  *@brief    д����
  *@param    data Ҫд�������
  *@retval   none
  */
int8_t mcuSpi::write(uint8_t data)
{
    __IO uint8_t dummyByte;
    while (LL_SPI_IsActiveFlag_TXE(_spi) == RESET)
        ;
    _spi->DR = data;
    while (LL_SPI_IsActiveFlag_RXNE(_spi) == RESET)
        ;
    dummyByte = _spi->DR;

    return 0;
}
/**
  *@brief    ����д����
  *@param    data Ҫд������ݣ�data_length ����
  *@retval   none
  */
int8_t mcuSpi::write_buf(uint8_t *data, uint16_t len)
{
    __IO uint8_t dummyByte;
    if(len == 0)
        return -1;
    while(len--)
    {
        while (LL_SPI_IsActiveFlag_TXE(_spi) == RESET)
            ;
        _spi->DR = *data++;
        while (LL_SPI_IsActiveFlag_RXNE(_spi) == RESET)
            ;
        dummyByte = _spi->DR;
    }
    return 0;
}
uint8_t mcuSpi::read()
{
        while (LL_SPI_IsActiveFlag_TXE(_spi) == RESET)
        ;
    _spi->DR = 0xff;
        while (LL_SPI_IsActiveFlag_RXNE(_spi) == RESET)
        ;
    return(_spi->DR);

}
int8_t mcuSpi::read(uint8_t *recv_data)
{
    while (LL_SPI_IsActiveFlag_TXE(_spi) == RESET)
        ;
    _spi->DR = 0xff;
    while (LL_SPI_IsActiveFlag_RXNE(_spi) == RESET)
        ;
    *recv_data = _spi->DR;

    return 0;
}

/**
  *@brief    ������ȡ����
  *@param    recv_data ��ȡ�������ݣ�data_length Ҫ��ȡ�����ݳ���
  *@retval   ���ص�ǰ�豸id
  */
int8_t mcuSpi::read_buf(uint8_t *recv_data, uint16_t len)
{
    if(len == 0)
        return -1;
    while(len--)
    {
        while (LL_SPI_IsActiveFlag_TXE(_spi) == RESET)
            ;
        _spi->DR = 0xff;
        while (LL_SPI_IsActiveFlag_RXNE(_spi) == RESET)
            ;
        *recv_data++ = _spi->DR;
    }
    return 0;
}

/**
  *@brief    ��ȡ����Ȩ
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::take(Spi::Config_t *newConfig)
{
    uint32_t end = GetEndTime(200);

    while (_busy == 1)
    {
        delay_ms(1);
        if (IsTimeOut(end, 200))
        {
            mcuSpiDebugErr("\r\nSPI�������߳��쳣����:%d\r\n",newConfig->dev_num);
            return EWAIT;
        }
    }
    if (newConfig->dev_num != read_config()) 
        config(newConfig);
    _busy = 1;
    return EOK;
}
/**
  *@brief    �ͷſ���Ȩ
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::release(void)
{
    _busy = 0;
    return 0;
}



