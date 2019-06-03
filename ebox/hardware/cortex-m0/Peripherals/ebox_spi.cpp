/**
  ******************************************************************************
  * @file    spi.cpp
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
/* Includes ------------------------------------------------------------------*/
#include "ebox_spi.h"
//#include "stm32f072_define.h"
#include "mcu_define.h"

#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  SPI_DEBUG(...) DBG("[SPI]  "),DBG(__VA_ARGS__)
#else
#define  SPI_DEBUG(...)
#endif

mcuSpi::mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    _busy = 0;
    _spi = SPIx;
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
}

void mcuSpi::begin(Config_t *spi_config)
{
    uint8_t index = 0;

    index = getIndex(_sck->id, SPI_MAP);
    _sck->mode(SPI_MAP[index]._pinMode, SPI_MAP[index]._pinAf);
    index = getIndex(_miso->id, SPI_MAP);
    _miso->mode(SPI_MAP[index]._pinMode, SPI_MAP[index]._pinAf);
    index = getIndex(_mosi->id, SPI_MAP);
    _mosi->mode(SPI_MAP[index]._pinMode, SPI_MAP[index]._pinAf);

    rcc_clock_cmd((uint32_t)_spi, ENABLE);
    config(spi_config);
}



void mcuSpi::config(Config_t *spi_config)
{
    current_dev_num = spi_config->dev_num;
    SPI_DEBUG("dev num 0x%x \r\n", current_dev_num);
    LL_SPI_Disable(_spi);
		uint32_t  prescaler;

    switch (spi_config->mode)
    {
    case MODE0:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        break;
    case MODE1:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        break;
    case MODE2:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        break;
    case MODE3:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        break;
    default:
        break;
    }

    switch(spi_config->prescaler)
    {
    case DIV2:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV2;
        break;
    case DIV4:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV4;
        break;
    case DIV8:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV8;
        break;
    case DIV16:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV16;
        break;
    case DIV32:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV32;
        break;
    case DIV64:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV64;
        break;
    case DIV128:
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV128;
        break;
    default :
        prescaler = LL_SPI_BAUDRATEPRESCALER_DIV256;
        break;

    }

    /* Configure SPI1 communication */
    LL_SPI_SetBaudRatePrescaler(_spi, prescaler);
    LL_SPI_SetTransferBitOrder(_spi, spi_config->bit_order);

    LL_SPI_SetTransferDirection(_spi, LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(_spi, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_SetNSSMode(_spi, LL_SPI_NSS_SOFT);
    LL_SPI_SetRxFIFOThreshold(_spi, LL_SPI_RX_FIFO_TH_QUARTER);

    LL_SPI_SetMode(_spi, LL_SPI_MODE_MASTER);
    LL_SPI_Enable(_spi);
}

/**
  *@brief    ��ȡ������Ϣ
  *@param    void
  *@retval   ���ص�ǰ�豸id
  */
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
    while ((_spi->SR & LL_SPI_SR_TXE) == RESET);
    *((__IO uint8_t *)&_spi->DR) = data;
    while ((_spi->SR & LL_SPI_SR_RXNE) == RESET);
    return (uint8_t)_spi->DR;
}
/**
  *@brief    д����
  *@param    data Ҫд�������
  *@retval   none
  */
int8_t mcuSpi::write(uint8_t data)
{
    return transfer(data);
}

/**
  *@brief    ����д����
  *@param    data Ҫд������ݣ�data_length ����
  *@retval   none
  */
int8_t mcuSpi::write_buf(uint8_t *data, uint16_t data_length)
{
    __IO uint8_t dummyByte;
    if (data_length == 0)
        return -1;
    while (data_length--)
    {
        transfer(*data++);
    }
    return 0;
}

uint8_t mcuSpi::read()
{
    return transfer(0xff);

}

int8_t mcuSpi::read(uint8_t *recv_data)
{
    *recv_data = transfer(0xff);
    return 0;
}

/**
  *@brief    ������ȡ����
  *@param    recv_data ��ȡ�������ݣ�data_length Ҫ��ȡ�����ݳ���
  *@retval   ���ص�ǰ�豸id
  */
int8_t mcuSpi::read_buf(uint8_t *recv_data, uint16_t data_length)
{
    if (data_length == 0)
        return -1;
    while (data_length--)
    {
        *recv_data++ = transfer(0xff);
    }
    return 0;
}

/**
  *@brief    ��ȡ����Ȩ
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::take(Config_t *spi_config)
{
    while((_busy == 1) && (spi_config->dev_num != read_config()))
        delay_ms(1);
    if(spi_config->dev_num == read_config())
    {
        _busy = 1;
        return 0;
    }
    config(spi_config);
    _busy = 1;
    return 0;
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



