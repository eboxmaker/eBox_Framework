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
// 是否打印调试信息, 1打印,0不打印
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
  *@brief    读取配置信息
  *@param    void
  *@retval   返回当前设备id
  */
uint8_t mcuSpi::read_config(void)
{
    return current_dev_num;
}

/**
  *@brief    transfer
  *@param    data 要发送的数据
  *@retval   uint8_t 接受到的数据
  */
uint8_t mcuSpi::transfer(uint8_t data)
{
    while ((_spi->SR & LL_SPI_SR_TXE) == RESET);
    *((__IO uint8_t *)&_spi->DR) = data;
    while ((_spi->SR & LL_SPI_SR_RXNE) == RESET);
    return (uint8_t)_spi->DR;
}
/**
  *@brief    写数据
  *@param    data 要写入的数据
  *@retval   none
  */
int8_t mcuSpi::write(uint8_t data)
{
    return transfer(data);
}

/**
  *@brief    连续写数据
  *@param    data 要写入的数据，data_length 长度
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
  *@brief    连续读取数据
  *@param    recv_data 读取到的数据，data_length 要读取的数据长度
  *@retval   返回当前设备id
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
  *@brief    获取控制权
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
  *@brief    释放控制权
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::release(void)
{
    _busy = 0;
    return 0;
}



