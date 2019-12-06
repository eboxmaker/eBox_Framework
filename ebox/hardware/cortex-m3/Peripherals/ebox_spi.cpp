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
#include "ebox_spi.h"
#include "dma.h"

    uint8_t tx_buffer[512];
    uint8_t rx_buffer[512];
#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#define  SPI_DEBUG(...) DBG("[SPI]  "),DBG(__VA_ARGS__)
#else
#define  SPI_DEBUG(...)
#endif

/*******************************************************************************
* Function Name  : SPI1_DMA_Configuration
* Description    : ����SPI1_RX��DMAͨ��2��SPI1_TX��DMAͨ��3
* Input          : None
* Output         : None
* Return         : None
* Attention             : 
*******************************************************************************/
void mcuSpi::dma_config( void )
{
    Dma1Ch2.rcc_enable();
    Dma1Ch2.nvic(ENABLE, 0, 0);
    Dma1Ch2.interrupt(DmaItTc, ENABLE);
    Dma1Ch2.interrupt(DmaItTe, DISABLE);
    Dma1Ch2.interrupt(DmaItHt, DISABLE);
    
    dmaRxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaRxCfg.DMA_MemoryBaseAddr = (uint32_t) rx_buffer;
    dmaRxCfg.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaRxCfg.DMA_BufferSize = 1;
    dmaRxCfg.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaRxCfg.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmaRxCfg.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dmaRxCfg.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaRxCfg.DMA_Mode = DMA_Mode_Normal;
    dmaRxCfg.DMA_Priority = DMA_Priority_High;
    dmaRxCfg.DMA_M2M = DMA_M2M_Disable;

      /* Enable SPI1 DMA RX request */
      SPI1->CR2 |= 1<<0;                                                                 //���ջ�����DMAʹ��
    
    
    
    
    
    Dma1Ch3.rcc_enable();
    Dma1Ch3.nvic(ENABLE, 0, 0);
    Dma1Ch3.interrupt(DmaItTc, ENABLE);
    Dma1Ch3.interrupt(DmaItTe, DISABLE);
    Dma1Ch3.interrupt(DmaItHt, DISABLE);

    
    dmaTxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaTxCfg.DMA_MemoryBaseAddr = (uint32_t) tx_buffer;
    dmaTxCfg.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaTxCfg.DMA_BufferSize = 1;
    dmaTxCfg.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    dmaTxCfg.DMA_MemoryInc = DMA_MemoryInc_Enable;
    dmaTxCfg.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    dmaTxCfg.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    dmaTxCfg.DMA_Mode = DMA_Mode_Normal;
    dmaTxCfg.DMA_Priority = DMA_Priority_High;
    dmaTxCfg.DMA_M2M = DMA_M2M_Disable;
   
    /* Enable SPI1 DMA TX request */
    SPI1->CR2 |= 1<<1;                                                                //���ͻ�����DMAʹ��
}


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
    dma_config();
}
void mcuSpi::config(Config_t *newConfig)
{
    SPI_InitTypeDef SPI_InitStructure;

    current_dev_num = newConfig->dev_num;


    SPI_Cmd(_spi, DISABLE);

    SPI_I2S_DeInit(_spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����ģʽ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�������
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC����ʽ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master; //����ģʽ

    if(newConfig->mode == MODE0)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(newConfig->mode == MODE1)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    else if(newConfig->mode == MODE2)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(newConfig->mode == MODE3)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    switch(newConfig->prescaler)
    {
    case DIV2:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        break;
    case DIV4:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
        break;
    case DIV8:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        break;
    case DIV16:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
        break;
    case DIV32:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
        break;
    case DIV64:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        break;
    case DIV128:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
        break;
    case DIV256:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;
    default :
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;

    }

    switch(newConfig->bit_order)
    {
    case MSB:
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        break;
    case LSB:
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
        break;
    default :
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        break;

    }
    SPI_Init(_spi, &SPI_InitStructure);
    SPI_Cmd(_spi, ENABLE);

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
    printf("\n===========err\n");
    while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    _spi->DR = data;
    while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
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
    tx_buffer[0] = data;
//     tx_buffer[0] = SPI1->DR ;
    //����ǰ��һ��SPI1->DR����֤���ջ�����Ϊ��
//    while( ( SPI1->SR & 0x02 ) == 0 );
       
//    DMA_InitTypeDef DMA_InitStructure;

//    Dma1Ch2.deInit();
//    Dma1Ch3.deInit();
    
    dmaTxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaTxCfg.DMA_MemoryBaseAddr = (uint32_t) tx_buffer;
    dmaTxCfg.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaTxCfg.DMA_BufferSize = 1;
    Dma1Ch3.init(&dmaTxCfg);
    
    dmaRxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaRxCfg.DMA_MemoryBaseAddr = (uint32_t) rx_buffer;
    dmaRxCfg.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaRxCfg.DMA_BufferSize = 1;
    Dma1Ch2.init(&dmaRxCfg);
    
    DMA1->IFCR = 0xF0 ;    //���ͨ��2�ı�־λ
    DMA1->IFCR = 0xF00 ;    //���ͨ��3�ı�־λ
    
    Dma1Ch2.enable();
    Dma1Ch3.enable();
    
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Tx, ENABLE) ;
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Rx, ENABLE) ;
    Dma1Ch2.wait();
    Dma1Ch3.wait();
    while(SPI1->SR & 0X80);
    DMA_Cmd(DMA1_Channel2, DISABLE); 
    DMA_Cmd(DMA1_Channel3, DISABLE); 
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Tx, DISABLE) ;
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Rx, DISABLE) ;
    return rx_buffer[0];
}
/**
  *@brief    ����д����
  *@param    data Ҫд������ݣ�data_length ����
  *@retval   none
  */
int8_t mcuSpi::write_buf(uint8_t *data, uint16_t len)
{
    printf("[spi]write_buf(data,%d)\n",len);
    dmaTxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaTxCfg.DMA_MemoryBaseAddr = (uint32_t) data;
    dmaTxCfg.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaTxCfg.DMA_BufferSize = len;
    Dma1Ch3.init(&dmaTxCfg);
    
    dmaRxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaRxCfg.DMA_MemoryBaseAddr = (uint32_t) rx_buffer;
    dmaRxCfg.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaRxCfg.DMA_BufferSize = len;
    Dma1Ch2.init(&dmaRxCfg);
    
    DMA1->IFCR = 0xF0 ;    //���ͨ��2�ı�־λ
    DMA1->IFCR = 0xF00 ;    //���ͨ��3�ı�־λ
    
    Dma1Ch2.enable();
    Dma1Ch3.enable();
    
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Tx, ENABLE) ;
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Rx, ENABLE) ;
    Dma1Ch2.wait();
    Dma1Ch3.wait();
    while(SPI1->SR & 0X80);
    DMA_Cmd(DMA1_Channel2, DISABLE); 
    DMA_Cmd(DMA1_Channel3, DISABLE); 
}
uint8_t mcuSpi::read()
{
//    //���ͨ��3�ı�־λ
//    tx_buffer[0] = SPI1->DR ;
//    //����ǰ��һ��SPI1->DR����֤���ջ�����Ϊ��
//    while( ( SPI1->SR & 0x02 ) == 0 );
    
//    DMA_InitTypeDef DMA_InitStructure;

//    Dma1Ch2.deInit();
//    Dma1Ch3.deInit();
//     while(SPI1->SR & 0X80);
//    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Tx, DISABLE) ;
//    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Rx, DISABLE) ;
    
    dmaTxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaTxCfg.DMA_MemoryBaseAddr = (uint32_t) tx_buffer;
    dmaTxCfg.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaTxCfg.DMA_BufferSize = 1;
    Dma1Ch3.init(&dmaTxCfg);
    
    dmaRxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaRxCfg.DMA_MemoryBaseAddr = (uint32_t) rx_buffer;
    dmaRxCfg.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaRxCfg.DMA_BufferSize = 1;
    Dma1Ch2.init(&dmaRxCfg);
    
    DMA1->IFCR = 0xF0 ;    //���ͨ��2�ı�־λ
    DMA1->IFCR = 0xF00 ;    //���ͨ��3�ı�־λ
    
    Dma1Ch2.enable();
    Dma1Ch3.enable();
    
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Tx, ENABLE) ;
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Rx, ENABLE) ;
    Dma1Ch2.wait();
    Dma1Ch3.wait();
    while(SPI1->SR & 0X80);
    DMA_Cmd(DMA1_Channel2, DISABLE); 
    DMA_Cmd(DMA1_Channel3, DISABLE); 

    return rx_buffer[0];

}
int8_t mcuSpi::read(uint8_t *recv_data)
{
    printf("\n===========err\n");
    while ((_spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    _spi->DR = 0xff;
    while ((_spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
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
    printf("[spi]read_buf(recv_data,%d)\n",len);
    dmaTxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaTxCfg.DMA_MemoryBaseAddr = (uint32_t) tx_buffer;
    dmaTxCfg.DMA_DIR = DMA_DIR_PeripheralDST;
    dmaTxCfg.DMA_BufferSize = len;
    Dma1Ch3.init(&dmaTxCfg);
    
    dmaRxCfg.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
    dmaRxCfg.DMA_MemoryBaseAddr = (uint32_t) recv_data;
    dmaRxCfg.DMA_DIR = DMA_DIR_PeripheralSRC;
    dmaRxCfg.DMA_BufferSize = len;
    Dma1Ch2.init(&dmaRxCfg);
    
    DMA1->IFCR = 0xF0 ;    //���ͨ��2�ı�־λ
    DMA1->IFCR = 0xF00 ;    //���ͨ��3�ı�־λ
    
    Dma1Ch2.enable();
    Dma1Ch3.enable();
    
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Tx, ENABLE) ;
    SPI_I2S_DMACmd( SPI1, SPI_I2S_DMAReq_Rx, ENABLE) ;
    Dma1Ch2.wait();
    Dma1Ch3.wait();
    while(SPI1->SR & 0X80);
    DMA_Cmd(DMA1_Channel2, DISABLE); 
    DMA_Cmd(DMA1_Channel3, DISABLE); 
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
            ebox_printf("\r\nSPI�������߳��쳣����\r\n");
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



