
#include "ebox_slave_spi.h"


class	SlaveSpi
{
public:
    SlaveSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi);

     void    begin (SpiConfig_t *spi_config);
     void    config(SpiConfig_t *spi_config);
    void    interrupt(FunctionalState enable,uint8_t preemption_priority, uint8_t sub_priority);

    void    enable_dma_send(uint8_t *buffer,uint16_t len);
//    virtual uint8_t read_config(void);

//    virtual uint8_t transfer(uint8_t data);

//    virtual int8_t  write(uint8_t data);
//    virtual int8_t  write(uint8_t *data, uint16_t data_length);

//    virtual uint8_t read();
//    virtual int8_t  read(uint8_t  *recv_data);
//    virtual int8_t  read(uint8_t *recv_data, uint16_t data_length);
//public:
//    virtual int8_t take_spi_right(SpiConfig_t *spi_config);
//    virtual int8_t release_spi_right(void);
private:
    SPI_TypeDef *spi;
    uint8_t     busy;
    DMA_Channel_TypeDef *_DMA1_Channelx;

};

SlaveSpi::SlaveSpi(SPI_TypeDef *SPIx, Gpio *sck, Gpio *miso, Gpio *mosi)
{
    busy = 0;
    spi = SPIx;
    sck->mode(AF_PP);
    miso->mode(AF_PP);
    mosi->mode(AF_PP);
}

void SlaveSpi::begin (SpiConfig_t *spi_config)
{
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

    config(spi_config);

}
void SlaveSpi::config(SpiConfig_t *spi_config)
{
    SPI_InitTypeDef SPI_InitStructure;



    SPI_Cmd(spi, DISABLE);

    SPI_I2S_DeInit(spi);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //ȫ˫��
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; //8λ����ģʽ
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; //NSS�������
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
    
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Rx,ENABLE);
    SPI_I2S_DMACmd(spi,SPI_I2S_DMAReq_Tx,ENABLE);
    
    SPI_I2S_ITConfig(spi,SPI_I2S_IT_RXNE,ENABLE);
    SPI_Cmd(spi, ENABLE);

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
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;

        case (uint32_t)SPI2_BASE:
            NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = preemption_priority;
            NVIC_InitStructure.NVIC_IRQChannelSubPriority = sub_priority;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            break;

    }
    NVIC_Init(&NVIC_InitStructure);

}

void SlaveSpi::enable_dma_send(uint8_t *buffer,uint16_t len)
{
    
    DMA_DeInit(_DMA1_Channelx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    _DMA1_Channelx->CPAR = (uint32_t)&spi->DR; //�����ַ
    _DMA1_Channelx->CMAR = (uint32_t) buffer; //mem��ַ
    _DMA1_Channelx->CNDTR = len ; //���䳤��
    _DMA1_Channelx->CCR = (0 << 14) | // �Ǵ洢�����洢��ģʽ
                          (2 << 12) | // ͨ�����ȼ���
                          (0 << 11) | // �洢�����ݿ��8bit
                          (0 << 10) | // �洢�����ݿ��8bit
                          (0 <<  9) | // �������ݿ��8bit
                          (0 <<  8) | // �������ݿ��8bit
                          (1 <<  7) | // �洢����ַ����ģʽ
                          (0 <<  6) | // �����ַ����ģʽ(����)
                          (0 <<  5) | // ��ѭ��ģʽ
                          (1 <<  4) | // �Ӵ洢����
                          (1 <<  3) | // �Ƿ�����������ж�
                          (0 <<  2) | // �Ƿ�����봫���ж�
                          (0 <<  1) | // �Ƿ�����������ж�
                          (1);        // ͨ������

}
