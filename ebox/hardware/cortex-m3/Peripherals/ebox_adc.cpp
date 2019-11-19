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
    //RCC_AHBPeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

    rcc_clock_cmd((uint32_t)ADCx, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M
    switch((uint32_t)ADCx)
    {
    case ADC1_BASE:
        dma = new Dma(DMA1_Channel1);//&Dma1Ch1;
        break;
    case ADC2_BASE:
        dma = new Dma(DMA1_Channel1);//&Dma1Ch1;
        break;
    }

    ADC_InitTypeDef  ADC_InitStructure;

    ADC_DeInit(ADCx);  //将外设 ADC1 的全部寄存器重设为缺省值

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换关闭
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = ch_num;	//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADCx, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    /* ADC1 regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期

    for(int i = 0; i < ch_num; i++)
    {
        if(ch_table[1][i] == ADC_Channel_TempSensor)
        {
            ADC_TempSensorVrefintCmd(ENABLE); //开启内部温度传感器
            ADC_RegularChannelConfig(ADCx, ch_table[1][i], i + 1, ADC_SampleTime_71Cycles5);
        }
        else
        {
            ADC_RegularChannelConfig(ADCx, ch_table[1][i], i + 1, ADC_SampleTime_28Cycles5);

        }
    }


    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADCx, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADCx, ENABLE);	   //使能指定的ADC1
    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADCx);	  //复位指定的ADC1的校准寄存器
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADCx));	//获取ADC1复位校准寄存器的状态,设置状态则等待

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADCx);		//开始指定ADC1的校准状态
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADCx));		//获取指定ADC1的校准程序,设置状态则等待


    dma->rcc_enable();
    dma->nvic(DISABLE, 0, 0);
    dma->interrupt(DmaItTc, DISABLE);
    dma->interrupt(DmaItTe, DISABLE);
    dma->interrupt(DmaItHt, DISABLE);




    dma->deInit();
    /* ADC1  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADCx->DR;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buf;  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = ch_num;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    dma->init(&DMA_InitStructure);
    dma->enable();

    ADC_SoftwareStartConvCmd(ADCx, ENABLE);
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
            return (adc_buf[i] * 3287.0 / 4096);
    }
    return 0;
}
float Adc::read_voltage(int index)
{
    return (adc_buf[index] * 3287.0 / 4096);
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

    temperate = (float)adc_value * (3287 / 4096.0); //电压值
    temperate = (1430 - temperate) / 4.35 + 24.958; //转换为温度值
    return temperate;
}
