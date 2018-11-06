#include "ebox_adc.h"

void Adc::add_temp_senser()
{
    ch_table[0][ch_num] = 0xff;//ADC_Channel_16
    ch_table[1][ch_num] = ADC_Channel_TempSensor;//ADC_Channel_16
    ch_num++;
}

void  Adc::add_ch(Gpio *io)
{
    io->mode(AIN);
    ch_table[0][ch_num] = (uint8_t)io->id;

    switch((uint32_t)io->id)
    {
    case PA0_ID:
        ch_table[1][ch_num] = ADC_Channel_0;
        break;
    case PA1_ID:
        ch_table[1][ch_num] = ADC_Channel_1;
        break;
    case PA2_ID:
        ch_table[1][ch_num] = ADC_Channel_2;
        break;
    case PA3_ID:
        ch_table[1][ch_num] = ADC_Channel_3;
        break;
    case PA4_ID:
        ch_table[1][ch_num] = ADC_Channel_4;
        break;
    case PA5_ID:
        ch_table[1][ch_num] = ADC_Channel_5;
        break;
    case PA6_ID:
        ch_table[1][ch_num] = ADC_Channel_6;
        break;
    case PA7_ID:
        ch_table[1][ch_num] = ADC_Channel_7;
        break;

    case PB0_ID:
        ch_table[1][ch_num] = ADC_Channel_8;
        break;
    case PB1_ID:
        ch_table[1][ch_num] = ADC_Channel_9;
        break;

    case PC0_ID:
        ch_table[1][ch_num] = ADC_Channel_10;
        break;
    case PC1_ID:
        ch_table[1][ch_num] = ADC_Channel_11;
        break;
    case PC2_ID:
        ch_table[1][ch_num] = ADC_Channel_12;
        break;
    case PC3_ID:
        ch_table[1][ch_num] = ADC_Channel_13;
        break;
    case PC4_ID:
        ch_table[1][ch_num] = ADC_Channel_14;
        break;
    case PC5_ID:
        ch_table[1][ch_num] = ADC_Channel_15;
        break;

    }
    ch_num++;
}
void  Adc::begin()
{

    //DMA configeration
    dma_cfg();

    //adc configeration
    adc_cfg();

    /* Enable DMA request after last transfer (Single-ADC mode) */
    ADC_DMARequestAfterLastTransferCmd(ADCx, ENABLE);

    /* Enable ADC3 DMA */
    ADC_DMACmd(ADCx, ENABLE);

    /* Enable ADC3 */
    ADC_Cmd(ADCx, ENABLE);



    ADC_SoftwareStartConv(ADCx);
}
void Adc::dma_cfg()
{
    switch((uint32_t)ADCx)
    {

    case ADC1_BASE:
        dma = &Dma2Stream0;
        dma_channel = DMA_Channel_0;
        break;//&Dma2Stream4;dma_channel = DMA_Channel_0;
    case ADC2_BASE:
        dma = &Dma2Stream2;
        dma_channel = DMA_Channel_1;;
        break;//&Dma2Stream3;dma_channel = DMA_Channel_1;
    case ADC3_BASE:
        dma = &Dma2Stream0;
        dma_channel = DMA_Channel_2;;
        break;//&Dma2Stream1;dma_channel = DMA_Channel_2;
    }


    dma->rcc_enable();
    dma->nvic(DISABLE, 0, 0);
    dma->interrupt(DmaItTc, DISABLE);
    dma->interrupt(DmaItTe, DISABLE);
    dma->interrupt(DmaItHt, DISABLE);



    dma->deInit();
    /* ADCx  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;

    DMA_InitStructure.DMA_Channel = dma_channel;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADCx->DR;
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&adc_buf;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    DMA_InitStructure.DMA_BufferSize = ch_num;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;


    dma->init(&DMA_InitStructure);
    dma->enable();

}
void Adc::adc_cfg()
{
    ADC_InitTypeDef  ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    //开启时钟
    rcc_clock_cmd((uint32_t)ADCx, ENABLE);


    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//ADC_DMAAccessMode_Disabled; //DMA失能
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//DISABLE;//非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;//关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStructure.ADC_NbrOfConversion = ch_num;//1;//1个转换在规则序列中 也就是只转换规则序列1
    ADC_Init(ADCx, &ADC_InitStructure);//ADC初始化

    /* ADCx regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADCx,ADC通道x,规则采样顺序值为y,采样时间为239.5周期
    for(int i = 0; i < ch_num; i++)
    {
        ADC_RegularChannelConfig(ADCx, ch_table[1][i], i + 1, ADC_SampleTime_15Cycles);
        if(ch_table[1][i] == ADC_Channel_TempSensor)
        {
            ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
        }
    }
}

uint16_t Adc::read(Gpio *io)
{
    for(int i = 0; i < ch_num; i++)
    {
        if(io->id == ch_table[0][i])
            return adc_buf[i];
    }
    return 0;
}
uint16_t Adc::read(int index)
{
    return adc_buf[index];
}

float Adc::read_voltage(Gpio *io)
{
    for(int i = 0; i < ch_num; i++)
    {
        if(io->id == ch_table[0][i])
            return (adc_buf[i] * 3300.0 / 4096);
    }
    return 0;
}
float Adc::read_voltage(int index)
{
    return (adc_buf[index] * 3300.0 / 4096);
}
float Adc::read_temp_senser()
{
    float temperate;
    uint16_t adc_value = 0;
    for(int i = 0; i < ch_num; i++)
    {
        if(0XFF == ch_table[0][i])
        {
            adc_value = adc_buf[i];
            break;
        }
    }

    temperate = (float)adc_value * (3300 / 4096.0); //电压值
    //    ebox_printf("Temp:%f\r\n",temperate);
    //    ebox_printf_flush();
    temperate = (temperate - 760) / 25 + 25; //转换为温度值
    return temperate;
}
