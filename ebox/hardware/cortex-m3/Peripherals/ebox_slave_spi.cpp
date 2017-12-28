
#include "ebox_slave_spi.h"

#include "ebox.h"

SlaveSpi::SlaveSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    busy = 0;
    spi = SPIx;
    sck->mode(AF_PP);
    miso->mode(AF_PP);
    mosi->mode(AF_PP);
}

void SlaveSpi::begin ()
{
    SpiConfig_t spi_config;
    spi_config.dev_num = 0;
    spi_config.bit_order = SPI_FirstBit_MSB;
    spi_config.mode = SPI_MODE0;
    spi_config.prescaler = SPI_BaudRatePrescaler_4;

    read_buf.begin(read_buf_pool,100);
    write_buf.begin(write_buf_pool,100);
    if(spi == SPI1)
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    }
    if(spi == SPI2)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    }
    if(spi == SPI3)
    {
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    }

    _DMA1_Channelx = DMA1_Channel3;
    config(&spi_config);

}
void SlaveSpi::config(SpiConfig_t *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;



    SPI_Cmd(spi, DISABLE);

    SPI_I2S_DeInit(spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //全双工
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8位数据模式
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS软件管理
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
    SPI_I2S_ITConfig(spi,SPI_I2S_IT_RXNE,ENABLE);
//    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Tx,ENABLE);
    interrupt(ENABLE,0,0);
    SPI_Cmd(spi, ENABLE);
    
    
    

}
void SlaveSpi::enable_rx_irq()
{
    
    SPI_I2S_ITConfig(spi,SPI_I2S_IT_RXNE,ENABLE);

}
void SlaveSpi::disable_rx_irq()
{
    
    SPI_I2S_ITConfig(spi,SPI_I2S_IT_RXNE,DISABLE);

}

void SlaveSpi::interrupt(FunctionalState enable,uint8_t preemption_priority, uint8_t sub_priority)
{
    if(preemption_priority > 3)preemption_priority = 3;
    if(sub_priority > 3)sub_priority = 3;
    NVIC_InitTypeDef NVIC_InitStructure;
    //SPI_I2S_ClearITPendingBit(spi, SPI_I2S_IT_RXNE);
    //USART_ClearFlag(USART2,USART_FLAG_TC); 
    switch((uint32_t)spi)
    {
        case (uint32_t)SPI1_BASE:
            NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
            NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
            break;

        case (uint32_t)SPI2_BASE:
            NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
            NVIC_InitStructure.NVIC_IRQChannelCmd = enable;
            break;

    }
    NVIC_Init(&NVIC_InitStructure);
    

}
uint8_t SlaveSpi::transfer(uint8_t data)
{
    while ((spi->SR & SPI_I2S_FLAG_TXE) == RESET)
        ;
    spi->DR = data;
    while ((spi->SR & SPI_I2S_FLAG_RXNE) == RESET)
        ;
    return spi->DR;
}

void SlaveSpi::enable_dma_send(uint8_t *buffer,uint16_t len)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
         RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA时钟
    DMA_Cmd(DMA1_Channel3,DISABLE);
    DMA_DeInit(DMA1_Channel3);   //将DMA的通道1寄存器重设为缺省值
    _DMA1_Channelx->CPAR = (uint32_t)&spi->DR; //外设地址
    _DMA1_Channelx->CMAR = (uint32_t) dma_buf; //mem地址
    _DMA1_Channelx->CNDTR = len ; //传输长度
    _DMA1_Channelx->CCR = (0 << 14) | // 0:非存储器到存储器模式,1:启动存储器到存储器模式。
                          (2 << 12) | // 通道优先级高0-3:低到高
                          (0 << 11) | // 存储器数据宽度8bit（0：8bit;1：16bit；2:32bit）
                          (0 << 10) | // 存储器数据宽度8bit（0：8bit;1：16bit；2:32bit）
                          (0 <<  9) | // 外设数据宽度8bit（0：8bit;1：16bit；2:32bit）
                          (0 <<  8) | // 外设数据宽度8bit（0：8bit;1：16bit；2:32bit）
                          (1 <<  7) | // 存储器地址增量模式（0：不执行存储器地址增量操作，1：执行存储器地址增量操作）
                          (0 <<  6) | // 外设地址增量模式(不增)（0：不执行外设地址增量操作，1：执行外设地址增量操作）
                          (0 <<  5) | // 非循环模式（0：不执行循环操作，1：执行循环操作）
                          (1 <<  4) | // 从存储器读（0：从外设读，1：从存储器读）
                          (1 <<  3) | // 是否允许传输错误中断（0：禁止TE中断，1：允许TE中断）
                          (1 <<  2) | // 是否允许半传输中断（0：禁止HT中断，1：允许HT中断）
                          (1 <<  1) | // 是否允许传输完成中断（0：禁止TC中断，1：允许TC中断）
                          (1);        // 通道开启（0：通道不工作，1：通道开启）
   DMA_ITConfig(DMA1_Channel3,DMA_IT_TC,ENABLE);
    DMA_Cmd(DMA1_Channel3,ENABLE);
}
void SlaveSpi::enable_dma_recv(uint8_t *buffer,uint16_t len)
{
    
    DMA_DeInit(DMA1_Channel2);   //将DMA的通道1寄存器重设为缺省值
    _DMA1_Channelx->CPAR = (uint32_t)&spi->DR; //外设地址
    _DMA1_Channelx->CMAR = (uint32_t) buffer; //mem地址
    _DMA1_Channelx->CNDTR = len ; //传输长度
    _DMA1_Channelx->CCR = (0 << 14) | // 非存储器到存储器模式
                          (2 << 12) | // 通道优先级高
                          (0 << 11) | // 存储器数据宽度8bit
                          (0 << 10) | // 存储器数据宽度8bit
                          (0 <<  9) | // 外设数据宽度8bit
                          (0 <<  8) | // 外设数据宽度8bit
                          (1 <<  7) | // 存储器地址增量模式
                          (0 <<  6) | // 外设地址增量模式(不增)
                          (0 <<  5) | // 非循环模式
                          (1 <<  4) | // 从存储器读
                          (1 <<  3) | // 是否允许传输错误中断
                          (0 <<  2) | // 是否允许半传输中断
                          (0 <<  1) | // 是否允许传输完成中断
                          (1);        // 通道开启

}

extern "C"
{
    void DMA1_Channel3_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC3) == SET)
        {
            slave_spi1.interrupt(ENABLE);

            uart1.println("DMA IT 3\r\n");
            DMA_ClearITPendingBit(DMA1_IT_TC3);
        }
    }
    void DMA1_Channel2_IRQHandler(void)
    {
        if(DMA_GetITStatus(DMA1_IT_TC2) == SET)
        {
            slave_spi1.interrupt(ENABLE);

            uart1.println("DMA IT 2\r\n");
            DMA_ClearITPendingBit(DMA1_IT_TC2);
        }
    }

    void SPI1_IRQHandler(void)
    {
        if(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE) == SET)
        {
            uint8_t ch = SPI1->DR;
            slave_spi1.write_buf.write(ch);
            uart1.println(ch);
//            slave_spi1.dma_buf = (uint8_t *)ebox_malloc(10);
            memset(slave_spi1.dma_buf,0x31,10);
            slave_spi1.enable_dma_send(slave_spi1.dma_buf,10);
            slave_spi1.interrupt(DISABLE);
        }
//        rt_interrupt_enter();
//        if (spi_is_rx_nonempty(spi1.c_dev())) {
//            spi_bridge_disable_spi_int();
//            rt_mb_send(spi_bridge.mb, spi1.read());
//            rx_count++;
//            spi_bridge_enable_spi_int();
//        }
//        rt_interrupt_leave();
    }
}
