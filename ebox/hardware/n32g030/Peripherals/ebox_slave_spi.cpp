
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


    read_buf.begin(read_buf_pool, 500);
    write_buf.begin(write_buf_pool, 500);
    cmd_buf.begin(cmd_buf_pool, 500);

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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟

    config(&spi_config);

}
void SlaveSpi::config(SpiConfig_t *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;



    SPI_Cmd(spi, DISABLE);

    SPI_I2S_DeInit(spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Hard; //NSS软件管理
    SPI_InitStructure.SPI_CRCPolynomial = 7; //CRC多项式
    SPI_InitStructure.SPI_Mode = SPI_Mode_Slave; //主机模式

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
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
        break;
    case SPI_CLOCK_DIV4:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
        break;
    case SPI_CLOCK_DIV8:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
        break;
    case SPI_CLOCK_DIV16:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
        break;
    case SPI_CLOCK_DIV32:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
        break;
    case SPI_CLOCK_DIV64:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
        break;
    case SPI_CLOCK_DIV128:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
        break;
    case SPI_CLOCK_DIV256:
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;
    default :
        SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
        break;

    }

    switch(spi_config->bit_order)
    {
    case MSB_FIRST:
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
        break;
    case LSB_FIRST:
        SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;
        break;
    }
    SPI_Init(spi, &SPI_InitStructure);


    nvic_dev_set_priority((uint32_t)spi, 0, 0, 0);
    nvic_dev_set_priority((uint32_t)dma_tx->get_dma_ch(), 0, 0, 0);
    nvic_dev_set_priority((uint32_t)dma_rx->get_dma_ch(), 0, 0, 0);


    nvic_dev_enable((uint32_t)spi, 0);
    nvic_dev_enable((uint32_t)dma_tx->get_dma_ch(), 0);
    nvic_dev_enable((uint32_t)dma_rx->get_dma_ch(), 0);
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

///////////////中断方式接收//////////////////////////////////////////////
void SlaveSpi::enable_rx_int()
{
    SPI_I2S_ITConfig(spi, SPI_I2S_IT_RXNE, ENABLE);

}
void SlaveSpi::disable_rx_int()
{
    SPI_I2S_ITConfig(spi, SPI_I2S_IT_RXNE, DISABLE);

}

//////////////DMA接收////////////////////////////////////////////////
void SlaveSpi::set_rx_dma()
{
    //    DMA_DeInit(DMA1_Channel2);   //将DMA的通道1寄存器重设为缺省值
    //    DMA1_Channel2->CPAR = (uint32_t)&spi->DR; //外设地址
    //    DMA1_Channel2->CMAR = (uint32_t) xfet; //mem地址
    //    DMA1_Channel2->CNDTR = xlen ; //传输长度
    //    DMA1_Channel2->CCR = (0 << 14) | // 非存储器到存储器模式
    //                          (2 << 12) | // 通道优先级高
    //                          (0 << 11) | // 存储器数据宽度8bit
    //                          (0 << 10) | // 存储器数据宽度8bit
    //                          (0 <<  9) | // 外设数据宽度8bit
    //                          (0 <<  8) | // 外设数据宽度8bit
    //                          (1 <<  7) | // 存储器地址增量模式
    //                          (0 <<  6) | // 外设地址增量模式(不增)
    //                          (0 <<  5) | // 非循环模式
    //                          (0 <<  4) | // 从存储器读
    //                          (0 <<  3) | // 是否允许传输错误中断
    //                          (0 <<  2) | // 是否允许半传输中断
    //                          (1 <<  1) | // 是否允许传输完成中断
    //                          (0);        // 通道开启

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
    dma_rx->attach(this, &SlaveSpi::dma_rx_over_event, DmaItTc);
    dma_rx->interrupt(DmaItTc, ENABLE);
}
void SlaveSpi::enable_rx_dma()
{
    SPI_I2S_DMACmd(spi, SPI_I2S_DMAReq_Rx, ENABLE);
    dma_rx->enable();

}

void SlaveSpi::disable_rx_dma()
{
    SPI_I2S_DMACmd(spi, SPI_I2S_DMAReq_Rx, DISABLE);
    dma_rx->disable();
}
void SlaveSpi::enable_dma_rx_int()
{
    dma_tx->interrupt(DmaItTc, ENABLE);
}

void SlaveSpi::disable_dma_rx_int()
{
    dma_tx->interrupt(DmaItTc, DISABLE);
}

////////////DMA 发送//////////////////////////////////////////////////////
void SlaveSpi::set_tx_dma()
{
    //    DMA_Cmd(DMA1_Channel3,DISABLE);
    //    DMA_DeInit(DMA1_Channel3);   //将DMA的通道1寄存器重设为缺省值
    //    _DMA1_Channelx->CPAR = (uint32_t)&spi->DR; //外设地址
    //    _DMA1_Channelx->CMAR = (uint32_t) xfet; //mem地址
    //    _DMA1_Channelx->CNDTR = xlen  ; //传输长度
    //    _DMA1_Channelx->CCR = (0 << 14) | // 0:非存储器到存储器模式,1:启动存储器到存储器模式。
    //                          (2 << 12) | // 通道优先级高0-3:低到高
    //                          (0 << 11) | // 存储器数据宽度8bit（0：8bit;1：16bit；2:32bit）
    //                          (0 << 10) | // 存储器数据宽度8bit（0：8bit;1：16bit；2:32bit）
    //                          (0 <<  9) | // 外设数据宽度8bit（0：8bit;1：16bit；2:32bit）
    //                          (0 <<  8) | // 外设数据宽度8bit（0：8bit;1：16bit；2:32bit）
    //                          (1 <<  7) | // 存储器地址增量模式（0：不执行存储器地址增量操作，1：执行存储器地址增量操作）
    //                          (0 <<  6) | // 外设地址增量模式(不增)（0：不执行外设地址增量操作，1：执行外设地址增量操作）
    //                          (0 <<  5) | // 非循环模式（0：不执行循环操作，1：执行循环操作）
    //                          (1 <<  4) | // 从存储器读（0：从外设读，1：从存储器读）
    //                          (0 <<  3) | // 是否允许传输错误中断（0：禁止TE中断，1：允许TE中断）
    //                          (0 <<  2) | // 是否允许半传输中断（0：禁止HT中断，1：允许HT中断）
    //                          (1 <<  1) | // 是否允许传输完成中断（0：禁止TC中断，1：允许TC中断）
    //                          (0);        // 通道开启（0：通道不工作，1：通道开启）

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
    dma_tx->attach(this, &SlaveSpi::dma_tx_over_event, DmaItTc);
    dma_tx->interrupt(DmaItTc, ENABLE);
}
void SlaveSpi::enable_tx_dma()
{
    SPI_I2S_DMACmd(spi, SPI_I2S_DMAReq_Tx, ENABLE);
    dma_tx->enable();

}
void SlaveSpi::disable_tx_dma()
{
    SPI_I2S_DMACmd(spi, SPI_I2S_DMAReq_Tx, DISABLE);
    dma_tx->disable();
}

void SlaveSpi::enable_dma_tx_int()
{
    dma_tx->interrupt(DmaItTc, ENABLE);
}

void SlaveSpi::disable_dma_tx_int()
{
    dma_tx->interrupt(DmaItTc, DISABLE);
}




void SlaveSpi::dma_rx_over_event()
{
    irq_handler(spi_irq_ids[SPI1_ID], SpiItRc);

}
void SlaveSpi::dma_tx_over_event()
{

    irq_handler(spi_irq_ids[SPI1_ID], SpiItTc);
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
    SlaveSpi *handler = (SlaveSpi *)id;
    handler->_irq[irq_type].call();
}


extern "C"
{


    void SPI1_IRQHandler(void)
    {
        if(SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == SET)
        {
            irq_handler(spi_irq_ids[SPI1_ID], SpiItRx);
        }

    }
    void spi_irq_handler(uint8_t index, SpiIrqHandler_t handler, uint32_t id)
    {
        irq_handler = handler;
        spi_irq_ids[index] = id;
    }

}
