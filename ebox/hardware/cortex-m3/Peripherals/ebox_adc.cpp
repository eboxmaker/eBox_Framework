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
        case PA0_ID:ch_table[1][ch_num] = ADC_Channel_0;break;
        case PA1_ID:ch_table[1][ch_num] = ADC_Channel_1;break;
        case PA2_ID:ch_table[1][ch_num] = ADC_Channel_2;break;
        case PA3_ID:ch_table[1][ch_num] = ADC_Channel_3;break;
        case PA4_ID:ch_table[1][ch_num] = ADC_Channel_4;break;
        case PA5_ID:ch_table[1][ch_num] = ADC_Channel_5;break;
        case PA6_ID:ch_table[1][ch_num] = ADC_Channel_6;break;
        case PA7_ID:ch_table[1][ch_num] = ADC_Channel_7;break;
        
        case PB0_ID:ch_table[1][ch_num] = ADC_Channel_8;break;
        case PB1_ID:ch_table[1][ch_num] = ADC_Channel_9;break;
        
        case PC0_ID:ch_table[1][ch_num] = ADC_Channel_10;break;
        case PC1_ID:ch_table[1][ch_num] = ADC_Channel_11;break;
        case PC2_ID:ch_table[1][ch_num] = ADC_Channel_12;break;
        case PC3_ID:ch_table[1][ch_num] = ADC_Channel_13;break;
        case PC4_ID:ch_table[1][ch_num] = ADC_Channel_14;break;
        case PC5_ID:ch_table[1][ch_num] = ADC_Channel_15;break;
            
    }
    ch_num++;
}
void  Adc::begin()
{
    rcc_clock_cmd((uint32_t)ADCx,ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M
    switch((uint32_t)ADCx)
    {
        case ADC1_BASE:dma = &Dma1Ch1;break;
        case ADC2_BASE:dma = &Dma1Ch1;break;
    }
    
    ADC_InitTypeDef  ADC_InitStructure;

    ADC_DeInit(ADCx);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת���ر�
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = ch_num;	//˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADCx, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    /* ADC1 regular channel11 configuration */
    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����

    for(int i = 0; i < ch_num; i++)
    {
        ADC_RegularChannelConfig(ADCx,ch_table[1][i],i+1,ADC_SampleTime_28Cycles5);
        if(ch_table[1][i] == ADC_Channel_TempSensor)
        {
            ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
        }
    }


    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADCx, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADCx, ENABLE);	   //ʹ��ָ����ADC1
    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADCx);	  //��λָ����ADC1��У׼�Ĵ���
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADCx));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADCx);		//��ʼָ��ADC1��У׼״̬
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADCx));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
    

    dma->rcc_enable();
    dma->nvic(DISABLE,0,0);
    dma->interrupt(DmaItTc,DISABLE);
    dma->interrupt(DmaItTe,DISABLE);
    dma->interrupt(DmaItHt,DISABLE);
    
    
    
    
    dma->deInit();
    /* ADC1  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADCx->DR;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buf;  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = ch_num;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
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
            return (adc_buf[i]* 3300.0 / 4096);
    }
    return 0;
}
float Adc::read_voltage(int index)
{
    return (adc_buf[index]* 3300.0 / 4096);
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

    temperate=(float)adc_value*(3.3/4096);       //��ѹֵ 
    temperate=(1.43-temperate)/0.0043+25;   //ת��Ϊ�¶�ֵ  
    return temperate;
}
