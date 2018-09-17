#include "ebox_adc.h"
#include "ebox_uart.h"

extern Uart uart1;

void Adc::add_temp_senser()
{
    ch_table[0][ch_num] = 0xff;//ADC_Channel_16
    ch_table[1][ch_num] = LL_ADC_CHANNEL_TEMPSENSOR;//ADC_Channel_16
    ch_num++;
}

void  Adc::add_ch(Gpio *io)
{
    io->mode(AIN);
    ch_table[0][ch_num] = (uint8_t)io->id;
    
    switch((uint32_t)io->id)
    {
        case PA0_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_0;break;
        case PA1_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_1;break;
        case PA2_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_2;break;
        case PA3_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_3;break;
        case PA4_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_4;break;
        case PA5_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_5;break;
        case PA6_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_6;break;
        case PA7_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_7;break;
        
        case PB0_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_8;break;
        case PB1_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_9;break;
        
        case PC0_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_10;break;
        case PC1_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_11;break;
        case PC2_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_12;break;
        case PC3_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_13;break;
        case PC4_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_14;break;
        case PC5_ID:ch_table[1][ch_num] = LL_ADC_CHANNEL_15;break;
            
    }
    ch_num++;
}
void  Adc::begin()
{
    rcc_clock_cmd((uint32_t)ADCx,ENABLE);
    //RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M
    switch((uint32_t)ADCx)
    {
        case ADC1_BASE:dma = &Dma1Ch1;break;
//        case ADC2_BASE:dma = &Dma1Ch1;break;
    }
    
    LL_ADC_InitTypeDef  ADC_InitStructure;

    LL_ADC_DeInit(ADCx);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.Clock = LL_ADC_CLOCK_SYNC_PCLK_DIV2;
    ADC_InitStructure.Resolution    = LL_ADC_RESOLUTION_12B;
    ADC_InitStructure.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    ADC_InitStructure.LowPowerMode  = LL_ADC_LP_MODE_NONE;
    
    LL_ADC_Init(ADCx,&ADC_InitStructure);
    
    
    LL_ADC_REG_InitTypeDef ADC_REG_InitStruct;
    
    ADC_REG_InitStruct.TriggerSource    = LL_ADC_REG_TRIG_SOFTWARE;         // ����Դ���������
    ADC_REG_InitStruct.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;   //
    ADC_REG_InitStruct.ContinuousMode   = LL_ADC_REG_CONV_CONTINUOUS;       // ��������
    ADC_REG_InitStruct.DMATransfer      = LL_ADC_REG_DMA_TRANSFER_NONE;
    ADC_REG_InitStruct.Overrun          = LL_ADC_REG_OVR_DATA_OVERWRITTEN;
    
    LL_ADC_REG_Init(ADCx,&ADC_REG_InitStruct);
    
    LL_ADC_SetSamplingTimeCommonChannels(ADC1, LL_ADC_SAMPLINGTIME_41CYCLES_5);
    for(int i = 0; i < ch_num; i++)
    {
        LL_ADC_REG_SetSequencerChAdd(ADCx,ch_table[1][i]);
        //ADC_RegularChannelConfig(ADCx,ch_table[1][i],i+1,ADC_SampleTime_28Cycles5);
        if(ch_table[1][i] == LL_ADC_CHANNEL_TEMPSENSOR)
        {
            //ADC_TempSensorVrefintCmd(ENABLE); //�����ڲ��¶ȴ�����
            LL_ADC_SetCommonPathInternalCh(__LL_ADC_COMMON_INSTANCE(ADC1),LL_ADC_PATH_INTERNAL_TEMPSENSOR);
            LL_ADC_REG_SetSequencerChAdd(ADCx,LL_ADC_CHANNEL_TEMPSENSOR);
        }
    }
    
    if (LL_ADC_IsEnabled(ADC1) == 0)
	{
		/* Run ADC self calibration */
		LL_ADC_StartCalibration(ADC1);
		while (LL_ADC_IsCalibrationOnGoing(ADC1) != 0){}
		/* Enable ADC */
		LL_ADC_Enable(ADC1);
		while (LL_ADC_IsActiveFlag_ADRDY(ADC1) == 0){}
	}
    enableDMA();
}
void Adc::enableDMA(){
    //    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
//    ADC_DMACmd(ADCx, ENABLE);

//    /* Enable ADC1 */
//    ADC_Cmd(ADCx, ENABLE);	   //ʹ��ָ����ADC1
//    /* Enable ADC1 reset calibaration register */
//    ADC_ResetCalibration(ADCx);	  //��λָ����ADC1��У׼�Ĵ���
//    /* Check the end of ADC1 reset calibration register */
//    while(ADC_GetResetCalibrationStatus(ADCx));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�

//    /* Start ADC1 calibaration */
//    ADC_StartCalibration(ADCx);		//��ʼָ��ADC1��У׼״̬
//    /* Check the end of ADC1 calibration */
//    while(ADC_GetCalibrationStatus(ADCx));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�
//    

    dma->rcc_enable();
    dma->nvic(ENABLE,0,0);
//    dma->interrupt(DmaItTc,DISABLE);
//    dma->interrupt(DmaItTe,DISABLE);
//    dma->interrupt(DmaItHt,DISABLE);
//    
//    
//    
//    
//    dma->deInit();
//    /* ADC1  DMA1 Channel Config */
//    DMA_InitTypeDef DMA_InitStructure;
//    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADCx->DR;  //DMA����ADC����ַ
//    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adc_buf;  //DMA�ڴ����ַ
//    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
//    DMA_InitStructure.DMA_BufferSize = ch_num;  //DMAͨ����DMA����Ĵ�С
//    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
//    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
//    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ
//    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
//    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
//    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
//    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
//    dma->init(&DMA_InitStructure);
//    dma->enable();

//    ADC_SoftwareStartConvCmd(ADCx, ENABLE);

	/* Enable the peripheral clock of DMA */
//	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	
//	if (_deep == 0){
//		r_buffer = (uint16_t*)malloc(_channelNum);
//		/* Set DMA transfer size */
//		LL_DMA_SetDataLength(DMA1,
//		                     LL_DMA_CHANNEL_1,
//		                     _channelNum);
//		LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
//	}else{
//		r_buffer = (uint16_t*)malloc(_channelNum*_deep);
		LL_DMA_SetDataLength(DMA1,
		                     LL_DMA_CHANNEL_1,
		                     ch_num);
		LL_ADC_REG_SetDMATransfer(ADC1, LL_ADC_REG_DMA_TRANSFER_UNLIMITED);
//	}

	/* Configure the DMA transfer */
	/*  - DMA transfer in circular mode to match with ADC configuration:        */
	/*    DMA unlimited requests.                                               */
	/*  - DMA transfer from ADC without address increment.                      */
	/*  - DMA transfer to memory with address increment.                        */
	/*  - DMA transfer from ADC by half-word to match with ADC configuration:   */
	/*    ADC resolution 12 bits.                                               */
	/*  - DMA transfer to memory by half-word to match with ADC conversion data */
	/*    buffer variable type: half-word.                                      */
	LL_DMA_ConfigTransfer(DMA1,
	                      LL_DMA_CHANNEL_1,
	                      LL_DMA_DIRECTION_PERIPH_TO_MEMORY |
	                      LL_DMA_MODE_CIRCULAR              |
	                      LL_DMA_PERIPH_NOINCREMENT         |
	                      LL_DMA_MEMORY_INCREMENT           |
	                      LL_DMA_PDATAALIGN_HALFWORD        |
	                      LL_DMA_MDATAALIGN_HALFWORD        |
	                      LL_DMA_PRIORITY_HIGH               );

	/* Set DMA transfer addresses of source and destination */
	LL_DMA_ConfigAddresses(DMA1,
	                       LL_DMA_CHANNEL_1,
	                       LL_ADC_DMA_GetRegAddr(ADC1, LL_ADC_DMA_REG_REGULAR_DATA),
	                       (uint32_t)adc_buf,
	                       LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

	/* Enable the DMA transfer */
	LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_1);
    
    	// ����ADC,���ȴ�����
	LL_ADC_REG_StartConversion(ADC1);
	while (!LL_DMA_IsActiveFlag_TC1(DMA1));
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
//        if(0XFF == ch_table[0][i])
//        {
//            adc_value = adc_buf[i];
//            break;
//        }
        
        if(LL_ADC_CHANNEL_TEMPSENSOR == ch_table[1][i])
        {
            adc_value = adc_buf[i];
            break;
        }
    }

    temperate=(float)adc_value*(3.3/4096);       //��ѹֵ 
    temperate=(1.43-temperate)/0.0043+25;   //ת��Ϊ�¶�ֵ 
    temperate = __LL_ADC_CALC_TEMPERATURE(3300,adc_value,LL_ADC_RESOLUTION_12B);
    return temperate;
}
