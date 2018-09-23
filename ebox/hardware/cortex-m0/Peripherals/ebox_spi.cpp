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

mcuSpi::mcuSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
	_spi = SPIx;
    _sck = sck;
    _miso = miso;
    _mosi = mosi;
}

void mcuSpi::begin(SpiConfig_t *spi_config)
{
    _sck->mode(AF_PP);
    _miso->mode(AF_PP);
    _mosi->mode(AF_PP);

    rcc_clock_cmd((uint32_t)_spi,ENABLE);
    config(spi_config);
}



void mcuSpi::config(SpiConfig_t *spi_config)
{
    LL_SPI_Disable(_spi);

    switch (spi_config->mode)
    {
    case SPI_MODE0:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        break;
    case SPI_MODE1:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_LOW);
        break;
    case SPI_MODE2:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_1EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        break;
    case SPI_MODE3:
        LL_SPI_SetClockPhase(_spi, LL_SPI_PHASE_2EDGE);
        LL_SPI_SetClockPolarity(_spi, LL_SPI_POLARITY_HIGH);
        break;
    default:
        break;
    }
    
    switch(spi_config->prescaler)
    {
        case SPI_CLOCK_DIV2:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV2;break;
        case SPI_CLOCK_DIV4:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV2;break;
        case SPI_CLOCK_DIV8:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV4;break;
        case SPI_CLOCK_DIV16:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV8;break;
        case SPI_CLOCK_DIV32:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV16;break;
        case SPI_CLOCK_DIV64:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV32;break;
        case SPI_CLOCK_DIV128:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV64;break;
        case SPI_CLOCK_DIV256:
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV128;break;
        default :
            spi_config->prescaler = LL_SPI_BAUDRATEPRESCALER_DIV256;break;

    }

    /* Configure SPI1 communication */
    LL_SPI_SetBaudRatePrescaler(_spi, spi_config->prescaler);
    LL_SPI_SetTransferBitOrder(_spi, spi_config->bit_order);

    LL_SPI_SetTransferDirection(_spi,LL_SPI_FULL_DUPLEX);
    LL_SPI_SetDataWidth(_spi, LL_SPI_DATAWIDTH_8BIT);
    LL_SPI_SetNSSMode(_spi, LL_SPI_NSS_SOFT);
	LL_SPI_SetRxFIFOThreshold(_spi, LL_SPI_RX_FIFO_TH_QUARTER);

    LL_SPI_SetMode(_spi, LL_SPI_MODE_MASTER);
    LL_SPI_Enable(_spi);
}

int8_t mcuSpi::write(uint8_t data)
{
	while(!LL_SPI_IsActiveFlag_TXE(_spi));
	*((__IO uint8_t *)&_spi->DR) = data;
//	LL_SPI_TransmitData8(_spi,data);
		while(!LL_SPI_IsActiveFlag_RXNE(_spi));
	return LL_SPI_ReceiveData8(_spi);
//	return 0;
}

int8_t mcuSpi::write(uint8_t *data, uint16_t data_length)
{
	__IO uint8_t dummyByte;
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		*((__IO uint8_t *)&_spi->DR) = *data;
	}
	return 0;
}

uint8_t mcuSpi::read()
{
	while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&_spi->DR) = 0xff;
//	LL_SPI_TransmitData8(_spi,0xff);
	while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	 return (LL_SPI_ReceiveData8(_spi));
	//return(_spi->DR);

}

int8_t mcuSpi::read(uint8_t *recv_data)
{
	while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&_spi->DR) = 0xff;
	while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	*recv_data = _spi->DR;

	return 0;
}

int8_t mcuSpi::read(uint8_t *recv_data, uint16_t data_length)
{
	if (data_length == 0)
		return -1;
	while (data_length--)
	{
		while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
			;
		*((__IO uint8_t *)&_spi->DR) = 0xff;
		while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
			;
		*recv_data = _spi->DR;
	}
	return 0;
}


//void mcuSpi::config(SpiConfig_t *spi_config)
//{
//    SPI_InitTypeDef SPI_InitStructure;

//    current_dev_num = spi_config->dev_num;


//    SPI_Cmd(spi, DISABLE);

//    SPI_I2S_DeInit(spi);
//    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
//    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
//    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
//    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
//    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //主机模式

//    if(spi_config->mode == SPI_MODE0)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//    }
//    else if(spi_config->mode == SPI_MODE1)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//    }
//    else if(spi_config->mode == SPI_MODE2)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
//    }
//    else if(spi_config->mode == SPI_MODE3)
//    {
//        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
//        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
//    }
//    switch(spi_config->prescaler)
//    {
//        case SPI_CLOCK_DIV2:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;break;
//        case SPI_CLOCK_DIV4:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;break;
//        case SPI_CLOCK_DIV8:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;break;
//        case SPI_CLOCK_DIV16:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;break;
//        case SPI_CLOCK_DIV32:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;break;
//        case SPI_CLOCK_DIV64:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;break;
//        case SPI_CLOCK_DIV128:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;break;
//        case SPI_CLOCK_DIV256:
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;break;
//        default :
//            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;break;

//    }
//    
//    switch(spi_config->bit_order)
//    {
//        case MSB_FIRST:
//            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;break;
//        case LSB_FIRST:
//            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;break;
//        default :
//            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;break;

//    }
//    SPI_Init(spi, &SPI_InitStructure);
//    SPI_Cmd(spi, ENABLE);

//}

uint8_t mcuSpi::read_config(void)
{
    return current_dev_num;
}

uint8_t mcuSpi::transfer(uint8_t data)
{
    while ((_spi->SR & LL_SPI_SR_TXE) == RESET)
		;
	*((__IO uint8_t *)&_spi->DR) = 0xff;
	while ((_spi->SR & LL_SPI_SR_RXNE) == RESET)
		;
	return   _spi->DR;
}

//int8_t mcuSpi::write(uint8_t data)
//{
//    __IO uint8_t dummyByte;
//    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
//        ;
//    spi->DR = data;
//    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
//        ;
//    dummyByte = spi->DR;

//    return 0;
//}
//int8_t mcuSpi::write(uint8_t *data, uint16_t len)
//{
//    __IO uint8_t dummyByte;
//    if(len == 0)
//        return -1;
//    while(len--)
//    {
//        while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
//            ;
//        spi->DR = *data++;
//        while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
//            ;
//        dummyByte = spi->DR;
//    }
//    return 0;
//}
//uint8_t mcuSpi::read()
//{
//    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
//        ;
//    spi->DR = 0xff;
//    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
//        ;
//    return(spi->DR);

//}
//int8_t mcuSpi::read(uint8_t *recv_data)
//{
//    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
//        ;
//    spi->DR = 0xff;
//    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
//        ;
//    *recv_data = spi->DR;

//    return 0;
//}

//int8_t mcuSpi::read(uint8_t *recv_data, uint16_t len)
//{
//    if(len == 0)
//        return -1;
//    while(len--)
//    {
//        while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
//            ;
//        spi->DR = 0xff;
//        while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
//            ;
//        *recv_data++ = spi->DR;
//    }
//    return 0;
//}

int8_t mcuSpi::take_spi_right(SpiConfig_t *spi_config)
{
    while((busy == 1) && (spi_config->dev_num != read_config()))
        delay_ms(1);
    if(spi_config->dev_num == read_config())
    {
        busy = 1;
        return 0;
    }
    config(spi_config);
    busy = 1;
    return 0;
}
int8_t mcuSpi::release_spi_right(void)
{
    busy = 0;
    return 0;
}



