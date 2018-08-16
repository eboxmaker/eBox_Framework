
#include "ebox_slave_spi.h"

static uint32_t spi_irq_ids[SPI_NUM] = {0};

static SpiIrqHandler_t irq_handler;


SlaveSpi::SlaveSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    spi = SPIx;
    sck->mode(AF_PP);
    miso->mode(AF_PP);
    mosi->mode(AF_PP);
    //PA4.mode(AF_PP);
}

void SlaveSpi::begin ()
{
    uint8_t index;
    SpiConfig_t spi_config;
    spi_config.dev_num = 0;
    spi_config.bit_order = SPI_FirstBit_MSB;
    spi_config.mode = SPI_MODE0;
    spi_config.prescaler = SPI_BaudRatePrescaler_8;

    
    read_buf.begin(read_buf_pool,500);
    write_buf.begin(write_buf_pool,500);
    cmd_buf.begin(cmd_buf_pool,500);
    
    if(spi == SPI1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
        index = SPI1_ID;
        dma_tx = &Dma1Ch3;
        dma_rx = &Dma1Ch2;

    }
    if(spi == SPI2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
        index = SPI2_ID;
    }
    if(spi == SPI3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
        index = SPI3_ID;
    }
    spi_irq_handler(index, SlaveSpi::_irq_handler, (uint32_t)this);
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMAʱ��

    config(&spi_config);

}
void SlaveSpi::config(SpiConfig_t *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;



    SPI_Cmd(spi, DISABLE);

    SPI_I2S_DeInit(spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����ģʽ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard; //NSS�������
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC����ʽ
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave; //����ģʽ

    if(spi_config->mode == SPI_MODE0)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(spi_config->mode == SPI_MODE1)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    else if(spi_config->mode == SPI_MODE2)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    }
    else if(spi_config->mode == SPI_MODE3)
    {
        SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
        SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    }
    switch(spi_config->prescaler)
    {
        case SPI_CLOCK_DIV2:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;break;
        case SPI_CLOCK_DIV4:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;break;
        case SPI_CLOCK_DIV8:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;break;
        case SPI_CLOCK_DIV16:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;break;
        case SPI_CLOCK_DIV32:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;break;
        case SPI_CLOCK_DIV64:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;break;
        case SPI_CLOCK_DIV128:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;break;
        case SPI_CLOCK_DIV256:
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;break;
        default :
            SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;break;

    }
    
    switch(spi_config->bit_order)
    {
        case MSB_FIRST:
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;break;
        case LSB_FIRST:
            SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;break;
    }
    SPI_Init(spi, &SPI_InitStructure);

    
    nvic_dev_set_priority((uint32_t)spi,0,0,0);
    nvic_dev_set_priority((uint32_t)dma_tx->get_dma_ch(),0,0,0);
    nvic_dev_set_priority((uint32_t)dma_rx->get_dma_ch(),0,0,0);
    
    
    nvic_dev_enable((uint32_t)spi,0);
    nvic_dev_enable((uint32_t)dma_tx->get_dma_ch(),0);
    nvic_dev_enable((uint32_t)dma_rx->get_dma_ch(),0);
    enable_rx_int();
    
    SPI_Cmd(spi, ENABLE);
}


void SlaveSpi::write(uint8_t data)
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET);
    spi->DR = data;
}

uint8_t SlaveSpi::read()
{
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET);
    return spi->DR;
}

///////////////�жϷ�ʽ����//////////////////////////////////////////////
void SlaveSpi::enable_rx_int()
{
    SPI_I2S_ITConfig(spi,SPI_I2S_IT_RXNE,ENABLE);

}
void SlaveSpi::disable_rx_int()
{
    SPI_I2S_ITConfig(spi,SPI_I2S_IT_RXNE,DISABLE);

}

//////////////DMA����////////////////////////////////////////////////
void SlaveSpi::set_rx_dma()
{
//    DMA_DeInit(DMA1_Channel2);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
//    DMA1_Channel2->CPAR = (uint32_t)&spi->DR; //�����ַ
//    DMA1_Channel2->CMAR = (uint32_t) xfet; //mem��ַ
//    DMA1_Channel2->CNDTR = xlen ; //���䳤��
//    DMA1_Channel2->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
//                          (2 << 12) | // ͨ�����ȼ���
//                          (0 << 11) | // �洢�����ݿ��8bit
//                          (0 << 10) | // �洢�����ݿ��8bit
//                          (0 <<  9) | // �������ݿ��8bit
//                          (0 <<  8) | // �������ݿ��8bit
//                          (1 <<  7) | // �洢����ַ����ģʽ
//                          (0 <<  6) | // �����ַ����ģʽ(����)
//                          (0 <<  5) | // ��ѭ��ģʽ
//                          (0 <<  4) | // �Ӵ洢����
//                          (0 <<  3) | // �Ƿ�����������ж�
//                          (0 <<  2) | // �Ƿ�����봫���ж�
//                          (1 <<  1) | // �Ƿ�����������ж�
//                          (0);        // ͨ������

    DMA_InitTypeDef DMA_InitStructure;

    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&spi->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) xfet;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = xlen;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    dma_tx->deInit();
    dma_rx->init(&DMA_InitStructure);
    dma_rx->attach(this,&SlaveSpi::dma_rx_over_event,DmaItTc);
    dma_rx->interrupt(DmaItTc,ENABLE);
}
void SlaveSpi::enable_rx_dma()
{
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Rx,ENABLE);
    dma_rx->enable();

}
    
void SlaveSpi::disable_rx_dma()
{
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Rx,DISABLE);
    dma_rx->disable();
}
void SlaveSpi::enable_dma_rx_int()
{
    dma_tx->interrupt(DmaItTc,ENABLE);
}

void SlaveSpi::disable_dma_rx_int()
{
    dma_tx->interrupt(DmaItTc,DISABLE);
}

////////////DMA ����//////////////////////////////////////////////////////
void SlaveSpi::set_tx_dma()
{
//    DMA_Cmd(DMA1_Channel3,DISABLE);
//    DMA_DeInit(DMA1_Channel3);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
//    _DMA1_Channelx->CPAR = (uint32_t)&spi->DR; //�����ַ
//    _DMA1_Channelx->CMAR = (uint32_t) xfet; //mem��ַ
//    _DMA1_Channelx->CNDTR = xlen  ; //���䳤��
//    _DMA1_Channelx->CCR = (0 << 14) | // 0:�Ǵ洢�����洢��ģʽ,1:�����洢�����洢��ģʽ��
//                          (2 << 12) | // ͨ�����ȼ���0-3:�͵���
//                          (0 << 11) | // �洢�����ݿ��8bit��0��8bit;1��16bit��2:32bit��
//                          (0 << 10) | // �洢�����ݿ��8bit��0��8bit;1��16bit��2:32bit��
//                          (0 <<  9) | // �������ݿ��8bit��0��8bit;1��16bit��2:32bit��
//                          (0 <<  8) | // �������ݿ��8bit��0��8bit;1��16bit��2:32bit��
//                          (1 <<  7) | // �洢����ַ����ģʽ��0����ִ�д洢����ַ����������1��ִ�д洢����ַ����������
//                          (0 <<  6) | // �����ַ����ģʽ(����)��0����ִ�������ַ����������1��ִ�������ַ����������
//                          (0 <<  5) | // ��ѭ��ģʽ��0����ִ��ѭ��������1��ִ��ѭ��������
//                          (1 <<  4) | // �Ӵ洢������0�����������1���Ӵ洢������
//                          (0 <<  3) | // �Ƿ�����������жϣ�0����ֹTE�жϣ�1������TE�жϣ�
//                          (0 <<  2) | // �Ƿ�����봫���жϣ�0����ֹHT�жϣ�1������HT�жϣ�
//                          (1 <<  1) | // �Ƿ�����������жϣ�0����ֹTC�жϣ�1������TC�жϣ�
//                          (0);        // ͨ��������0��ͨ����������1��ͨ��������

    DMA_InitTypeDef DMA_InitStructure;

    
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&spi->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) xfet;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = xlen;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    dma_tx->deInit();
    dma_tx->init(&DMA_InitStructure);
    dma_tx->attach(this,&SlaveSpi::dma_tx_over_event,DmaItTc);
    dma_tx->interrupt(DmaItTc,ENABLE);
}
void SlaveSpi::enable_tx_dma()
{
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Tx,ENABLE);
    dma_tx->enable();

}
void SlaveSpi::disable_tx_dma()
{
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Tx,DISABLE);
    dma_tx->disable();
}
    
void SlaveSpi::enable_dma_tx_int()
{
    dma_tx->interrupt(DmaItTc,ENABLE);
}

void SlaveSpi::disable_dma_tx_int()
{
    dma_tx->interrupt(DmaItTc,DISABLE);
}




void SlaveSpi::dma_rx_over_event()
{
    irq_handler(spi_irq_ids[SPI1_ID],SpiItRc);

}
void SlaveSpi::dma_tx_over_event()
{
    
    irq_handler(spi_irq_ids[SPI1_ID],SpiItTc);
}
void SlaveSpi::int_rx_event()
{

}
void SlaveSpi::attach(void (*fptr)(void), SpiIrqType type) 
{
    if (fptr) 
    {
        _irq[type].attach(fptr);
    }
}

void SlaveSpi::_irq_handler(uint32_t id, SpiIrqType irq_type)
{
    SlaveSpi *handler = (SlaveSpi*)id;
    handler->_irq[irq_type].call();
}


extern "C"
{


    void SPI1_IRQHandler(void)
    {
        if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == SET)
        {
            irq_handler(spi_irq_ids[SPI1_ID],SpiItRx);
        }

    }
void spi_irq_handler(uint8_t index, SpiIrqHandler_t handler, uint32_t id)
{
    irq_handler = handler;
    spi_irq_ids[index] = id;
}
    
}
