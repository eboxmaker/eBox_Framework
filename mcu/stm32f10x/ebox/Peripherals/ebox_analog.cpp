/**
  ******************************************************************************
  * @file    analog.cpp
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
#include "ebox_analog.h"

/** @defgroup analog 
  * @brief analog driver modulesĬ�Ͽ���16ͨ�� ����DMA+ADC����ת��ģʽ���ṩAD�ɼ�����ֻ�轫IO����ΪAIN
  * ģʽ���ɶ�ȡ������Ӧ�ĵ�ѹ
  * @{
  */

/** @defgroup analog˽�����Ͷ���
  * @{
  */

/**
  * @}
  */

/** @defgroup analog˽�к궨��
  * @{
  */  
  
#define CH 16/* ADC JEXTTRIG mask */

/**
  * @}
  */
  
  
/** @defgroup analog˽�б���
  * @{
  */
  
uint16_t  AD_value[CH];///< �������ADCת�������Ҳ��DMA��Ŀ���ַ
  
/**
  * @}
  */


/** @defgroup analog����
  * @{
  */

/**
 * @brief    ADC���ã���ADC1����Ϊ16ͨ�������������������28.5�����ڣ�10bit����
 * @param    NONE
 * @return   
 *           - 0: fail 
 *           - 1: succeed
 */
void ADC1_configuration(void)
{

    ADC_InitTypeDef  ADC_InitStructure;

    ADC_DeInit(ADC1);  //������ ADC1 ��ȫ���Ĵ�������Ϊȱʡֵ

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//ģ��ת��������ɨ��ģʽ
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//ģ��ת������������ת��ģʽ
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//�ⲿ����ת���ر�
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
    ADC_InitStructure.ADC_NbrOfChannel = CH;	//˳����й���ת����ADCͨ������Ŀ
    ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���

    /* ADC1 regular channel11 configuration */
    //����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��
    //ADC1,ADCͨ��x,�������˳��ֵΪy,����ʱ��Ϊ239.5����

    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 6, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 7, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 9, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_9, 10, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_10, 11, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 12, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_12, 13, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_13, 14, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_14, 15, ADC_SampleTime_28Cycles5 );
    ADC_RegularChannelConfig(ADC1, ADC_Channel_15, 16, ADC_SampleTime_28Cycles5 );

    // ����ADC��DMA֧�֣�Ҫʵ��DMA���ܣ������������DMAͨ���Ȳ�����
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);	   //ʹ��ָ����ADC1
    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADC1);	  //��λָ����ADC1��У׼�Ĵ���
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));	//��ȡADC1��λУ׼�Ĵ�����״̬,����״̬��ȴ�

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//��ʼָ��ADC1��У׼״̬
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));		//��ȡָ��ADC1��У׼����,����״̬��ȴ�

}
/**
 *@brief    ADC��DMA���ã���ADC1����ΪDMA�Զ�����ģʽ��ת������Զ��Ĵ��䵽�ڴ�
 *@param    NONE
 *@retval   NONE
*/
void DMA_configuration(void)
{
    /* ADC1  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADC1->DR;  //DMA����ADC����ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_value;  //DMA�ڴ����ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //�ڴ���Ϊ���ݴ����Ŀ�ĵ�
    DMA_InitStructure.DMA_BufferSize = CH;  //DMAͨ����DMA����Ĵ�С
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //�����ַ�Ĵ�������
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //�ڴ��ַ�Ĵ�������
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //������ѭ������ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMAͨ�� xӵ�и����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��

}

/**
 *@brief    ADC��ʼ��
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );	  //ʹ��ADC1ͨ��ʱ�ӣ������ܽ�ʱ��
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC���ʱ�䲻�ܳ���14M
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����

    ADC1_configuration();
    DMA_configuration();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);	 //����DMAͨ��


}
/**
 *@brief    ��ȡADC1ĳ�������ϵ�ģ��ת�����
 *@param    pin��ADC1ĳͨ��ӳ�������
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��10bit��ADCת�����
            ������Ŵ��󷵻�0��
*/
uint16_t analog_read(Gpio *pin)
{
    switch((uint32_t)pin->id)
    {
        case PA0_ID:
            return AD_value[0];
        case PA1_ID:
            return AD_value[1];
        case PA2_ID:
            return AD_value[2];
        case PA3_ID:
            return AD_value[3];
        case PA4_ID:
            return AD_value[4];
        case PA5_ID:
            return AD_value[5];
        case PA6_ID:
            return AD_value[6];
        case PA7_ID:
            return AD_value[7];
        case PB0_ID:
            return AD_value[8];
        case PB1_ID:
            return AD_value[9];
        case PC0_ID:
            return AD_value[10];
        case PC1_ID:
            return AD_value[11];
        case PC2_ID:
            return AD_value[12];
        case PC3_ID:
            return AD_value[13];
        case PC4_ID:
            return AD_value[14];
        case PC5_ID:
            return AD_value[15];
        default :
            break;
    }

    return 0;

}
/**
 *@brief    ��ȡĳ�������ϵ�ģ���ѹ
 *@param    pin��ADC1ĳͨ��ӳ�������
 *@retval   ���������ȷ�򷵻ظ����ŵ�ģ���ѹֵ����Ӧ��ģ���ѹ��Ĭ�ϲο���ѹΪ3.3V
            ������Ŵ��󷵻�0��
*/
uint16_t analog_read_voltage(Gpio *pin)
{
    return (uint16_t)(analog_read(pin) * 3300 / 4096); //��Ľ��������1000���������������С��
}

/**
  * @}
  */
