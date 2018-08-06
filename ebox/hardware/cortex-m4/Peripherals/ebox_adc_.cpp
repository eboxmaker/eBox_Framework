
#include "ebox_adc.h"


//Ĭ�Ͽ���16ͨ�� ����DMA+ADC����ת��ģʽ���ṩAD�ɼ�����
//ֻ�轫IO����ΪAINģʽ���ɶ�ȡ������Ӧ�ĵ�ѹ
#define CH 16
u16  AD_value[CH];   //�������ADCת�������Ҳ��DMA��Ŀ���ַ


/**
 *@name     void ADC1_configuration(void)
 *@brief    ADC���ã���ADC1����Ϊ16ͨ�������������������28.5�����ڣ�10bit����
 *@param    NONE
 *@retval   NONE
*/
void ADC1_configuration(void)
{

    ADC_InitTypeDef  ADC_InitStructure;
	  ADC_CommonInitTypeDef ADC_CommonInitStructure;

    ADC_DeInit();  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ
	
	  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1��λ
	  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//��λ����	

    /* ADC1 configuration ------------------------------------------------------*/

	  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//����ģʽ
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//���������׶�֮����ӳ�5��ʱ��
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//ADC_DMAAccessMode_Disabled; //DMAʧ��
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//Ԥ��Ƶ4��Ƶ��ADCCLK=PCLK2/4=84/4=21Mhz,ADCʱ����ò�Ҫ����36Mhz 
    ADC_CommonInit(&ADC_CommonInitStructure);//��ʼ��
	
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12λģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//DISABLE;//��ɨ��ģʽ	
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;//�ر�����ת��
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//��ֹ������⣬ʹ���������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//�Ҷ���	
    ADC_InitStructure.ADC_NbrOfConversion = CH;//1;//1��ת���ڹ��������� Ҳ����ֻת����������1 
    ADC_Init(ADC1, &ADC_InitStructure);//ADC��ʼ��

    /* ADC1 regular channel11 configuration */
    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 13, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 15, ADC_SampleTime_15Cycles );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 16, ADC_SampleTime_15Cycles );

    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
   
		ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);
		
    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);	   //ʹ��ָ����ADC1
    /* Enable ADC1 reset calibaration register */
   // ADC_ResetCalibration(ADC1);	  //��λָ����ADC1��У׼�Ĵ���
    /* Check the end of ADC1 reset calibration register */
   // while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�

    /* Start ADC1 calibaration */
   // ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬
    /* Check the end of ADC1 calibration */
  //  while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�

}
/**
 *@name     void DMA_configuration(void)
 *@brief    ADC��DMA���ã���ADC1����ΪDMA�Զ�����ģʽ��ת������Զ��Ĵ��䵽�ڴ�
 *@param    NONE
 *@retval   NONE
*/
void DMA_configuration(void)
{
    /* ADC1  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;
	
    DMA_DeInit(DMA2_Stream0);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
	  while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE){}//�ȴ�DMA������ 

		 /* ���� DMA Stream */
  DMA_InitStructure.DMA_Channel = DMA_Channel_0;//ͨ��ѡ��
  DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)&ADC1->DR;//DMA�����ַ
  DMA_InitStructure.DMA_Memory0BaseAddr =  (uint32_t)&AD_value;//DMA �洢��0��ַ
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//�洢��������ģʽ
  DMA_InitStructure.DMA_BufferSize = CH;//���ݴ����� 
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
  DMA_InitStructure.DMA_MemoryInc =  DMA_MemoryInc_Enable;//�洢������ģʽ
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//�������ݳ���:16λ
  DMA_InitStructure.DMA_MemoryDataSize =     DMA_MemoryDataSize_HalfWord;//�洢�����ݳ���:16λ
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// ʹ����ͨģʽ 
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;//�ߵ����ȼ�
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
  DMA_Init(DMA2_Stream0, &DMA_InitStructure);//��ʼ��DMA Stream	
	
	DMA_Cmd(DMA2_Stream0,ENABLE); 
			
}

/**
 *@name     void ADC1_init(void)
 *@brief    ADC��ʼ��
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );	  //ʹ��ADC1ͨ��ʱ�ӣ������ܽ�ʱ��
	  	
	  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA1ʱ��ʹ�� 

    ADC1_configuration();
    DMA_configuration();
	  ADC_SoftwareStartConv(ADC1);
	  
}
/**
 *@name     uint16_t analog_read(GPIO *pin)
 *@brief    ��ȡADC1ĳ�������ϵ�ģ��ת�����
 *@param    pin��ADC1ĳͨ��ӳ�������
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��10bit��ADCת�����
            ������Ŵ��󷵻�0��
*/
uint16_t analog_read(Gpio *pin)
{
//    switch((uint32_t)pin->port)
//    {
//    case  (uint32_t)GPIOA_BASE:
//        switch(pin->pin)
//        {
//        case GPIO_Pin_0:
//            return AD_value[0];
//        case GPIO_Pin_1:
//            return AD_value[1];
//        case GPIO_Pin_2:
//            return AD_value[2];
//        case GPIO_Pin_3:
//            return AD_value[3];
//        case GPIO_Pin_4:
//            return AD_value[4];
//        case GPIO_Pin_5:
//            return AD_value[5];
//        case GPIO_Pin_6:
//            return AD_value[6];
//        case GPIO_Pin_7:
//            return AD_value[7];
//        }
//    case (uint32_t)GPIOB_BASE:
//        switch(pin->pin)
//        {
//        case GPIO_Pin_0:
//            return AD_value[8];
//        case GPIO_Pin_1:
//            return AD_value[9];
//        }
//    case (uint32_t)GPIOC_BASE:
//        switch(pin->pin)
//        {
//        case GPIO_Pin_0:
//            return AD_value[10];
//        case GPIO_Pin_1:
//            return AD_value[11];
//        case GPIO_Pin_2:
//            return AD_value[12];
//        case GPIO_Pin_3:
//            return AD_value[13];
//        case GPIO_Pin_4:
//            return AD_value[14];
//        case GPIO_Pin_5:
//            return AD_value[15];
//        }
//    }

    return 0;

}
/**
 *@name     uint16_t analog_read_voltage(GPIO *pin)
 *@brief    ��ȡĳ�������ϵ�ģ���ѹ
 *@param    pin��ADC1ĳͨ��ӳ�������
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��ģ���ѹ��Ĭ�ϲο���ѹΪ3.3V
            ������Ŵ��󷵻�0��
*/
uint16_t analog_read_voltage(Gpio *pin)
{
    return (u16)(analog_read(pin) * 3300 / 4096); //��Ľ��������1000���������������С��
}








