/**
  ******************************************************************************
  * @file    _spi.cpp
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
#include "ebox_mcu_spi.h"
#include "dma.h"


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

/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
* Attention             : 
*******************************************************************************/

mcuSpi::mcuSpi(SPI_Module *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
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
    SPI_InitType SPI_InitStructure;

    current_dev_num = newConfig->dev_num;


    SPI_Enable(_spi, DISABLE);
    
    SPI_I2S_DeInit(_spi);
    SPI_InitStructure.DataDirection = SPI_DIR_DOUBLELINE_FULLDUPLEX; //ȫ˫��
    SPI_InitStructure.DataLen = SPI_DATA_SIZE_8BITS; //8λ����ģʽ
    SPI_InitStructure.NSS = SPI_NSS_SOFT; //NSS�������
    SPI_InitStructure.CRCPoly = 7; //CRC����ʽ
    SPI_InitStructure.SpiMode = SPI_MODE_MASTER; //����ģʽ

    if(newConfig->mode == MODE0)
    {
        SPI_InitStructure.CLKPOL = SPI_CLKPOL_LOW;
        SPI_InitStructure.CLKPHA = SPI_CLKPHA_FIRST_EDGE;
    }
    else if(newConfig->mode == MODE1)
    {
        SPI_InitStructure.CLKPOL = SPI_CLKPOL_LOW;
        SPI_InitStructure.CLKPHA = SPI_CLKPHA_SECOND_EDGE;
    }
    else if(newConfig->mode == MODE2)
    {
        SPI_InitStructure.CLKPOL = SPI_CLKPOL_HIGH;
        SPI_InitStructure.CLKPHA = SPI_CLKPHA_FIRST_EDGE;
    }
    else if(newConfig->mode == MODE3)
    {
        SPI_InitStructure.CLKPOL = SPI_CLKPOL_HIGH;
        SPI_InitStructure.CLKPHA = SPI_CLKPHA_SECOND_EDGE;
    }
    switch(newConfig->prescaler)
    {
    case DIV2:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_2;
        break;
    case DIV4:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_4;
        break;
    case DIV8:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_8;
        break;
    case DIV16:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_16;
        break;
    case DIV32:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_32;
        break;
    case DIV64:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_64;
        break;
    case DIV128:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_128;
        break;
    case DIV256:
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_256;
        break;
    default :
        SPI_InitStructure.BaudRatePres = SPI_BR_PRESCALER_256;
        break;

    }

    switch(newConfig->bit_order)
    {
    case MSB:
        SPI_InitStructure.FirstBit = SPI_FB_MSB;
        break;
    case LSB:
        SPI_InitStructure.FirstBit = SPI_FB_LSB;
        break;
    default :
        SPI_InitStructure.FirstBit = SPI_FB_MSB;
        break;

    }
    SPI_Init(_spi, &SPI_InitStructure);
    SPI_Enable(_spi, ENABLE);

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
    mcuSpiDebug("\n===========err\n");
    while ((_spi->STS & SPI_I2S_TE_FLAG) == RESET)
        ;
    _spi->DAT = data;
    while ((_spi->STS & SPI_I2S_RNE_FLAG) == RESET)
        ;
    return _spi->DAT;
}
/**
  *@brief    д����
  *@param    data Ҫд�������
  *@retval   none
  */
int8_t mcuSpi::write(uint8_t data)
{
    __IO uint8_t dummyByte;
    while ((_spi->STS & SPI_I2S_TE_FLAG) == RESET)
        ;
    _spi->DAT = data;
    while ((_spi->STS & SPI_I2S_RNE_FLAG) == RESET)
        ;
    dummyByte = _spi->DAT;

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
        while ((_spi->STS & SPI_I2S_TE_FLAG) == RESET)
            ;
        _spi->DAT = *data++;
        while ((_spi->STS & SPI_I2S_RNE_FLAG) == RESET)
            ;
        dummyByte = _spi->DAT;
    }
    return 0;
}
uint8_t mcuSpi::read()
{
    while ((_spi->STS & SPI_I2S_TE_FLAG) == RESET)
        ;
    _spi->DAT = 0xff;
    while ((_spi->STS & SPI_I2S_RNE_FLAG) == RESET)
        ;
    return(_spi->DAT);

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
        while ((_spi->STS & SPI_I2S_TE_FLAG) == RESET)
            ;
        _spi->DAT = 0xff;
        while ((_spi->STS & SPI_I2S_RNE_FLAG) == RESET)
            ;
        *recv_data++ = _spi->DAT;
    }
    return 0;
}

/**
  *@brief    ��ȡ����Ȩ
  *@param    none
  *@retval   none
  */
int8_t mcuSpi::take(Config_t *newConfig)
{
    uint32_t end = GetEndTime(200);

    while (_busy == 1)
    {
        delay_ms(1);
        if (IsTimeOut(end, 200))
        {
            ebox_printf("\nSPI�������߳��쳣����\n");
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



