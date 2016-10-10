/**
  ******************************************************************************
  * @file    analog.cpp
  * @author  shentq
  * @version V1.2
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
  * @brief analog driver modules默认开启16通道 采用DMA+ADC连续转换模式。提供AD采集服务，只需将IO设置为AIN
  * 模式即可读取引脚相应的电压
  * @{
  */

/** @defgroup analog私有类型定义
  * @{
  */

/**
  * @}
  */

/** @defgroup analog私有宏定义
  * @{
  */  
  
#define CH 16/* ADC JEXTTRIG mask */

/**
  * @}
  */
  
  
/** @defgroup analog私有变量
  * @{
  */
  
uint16_t  AD_value[CH];///< 用来存放ADC转换结果，也是DMA的目标地址
  
/**
  * @}
  */


/** @defgroup analog函数
  * @{
  */

/**
 * @brief    ADC配置，将ADC1配置为16通道规则采样，采样周期28.5个周期，10bit精度
 * @param    NONE
 * @return   
 *           - 0: fail 
 *           - 1: succeed
 */
void ADC1_configuration(void)
{

    ADC_InitTypeDef  ADC_InitStructure;

    ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值

    /* ADC1 configuration ------------------------------------------------------*/
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;	//模数转换工作在扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	//模数转换工作在连续转换模式
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//外部触发转换关闭
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
    ADC_InitStructure.ADC_NbrOfChannel = CH;	//顺序进行规则转换的ADC通道的数目
    ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

    /* ADC1 regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期

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

    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);	   //使能指定的ADC1
    /* Enable ADC1 reset calibaration register */
    ADC_ResetCalibration(ADC1);	  //复位指定的ADC1的校准寄存器
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1复位校准寄存器的状态,设置状态则等待

    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待

}
/**
 *@brief    ADC的DMA配置，将ADC1配置为DMA自动传输模式。转换结果自动的传输到内存
 *@param    NONE
 *@retval   NONE
*/
void DMA_configuration(void)
{
    /* ADC1  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel1);   //将DMA的通道1寄存器重设为缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr =  (uint32_t)&ADC1->DR;  //DMA外设ADC基地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&AD_value;  //DMA内存基地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //内存作为数据传输的目的地
    DMA_InitStructure.DMA_BufferSize = CH;  //DMA通道的DMA缓存的大小
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  //外设地址寄存器不变
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  //内存地址寄存器递增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //数据宽度为16位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //数据宽度为16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  //工作在循环缓存模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High; //DMA通道 x拥有高优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;  //DMA通道x没有设置为内存到内存传输
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);  //根据DMA_InitStruct中指定的参数初始化DMA的通道

}

/**
 *@brief    ADC初始化
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 , ENABLE );	  //使能ADC1通道时钟，各个管脚时钟
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //72M/6=12,ADC最大时间不能超过14M
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//使能DMA传输

    ADC1_configuration();
    DMA_configuration();
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE);	 //启动DMA通道


}
/**
 *@brief    读取ADC1某个引脚上的模拟转换结果
 *@param    pin：ADC1某通道映射的引脚
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的10bit的ADC转换结果
            如果引脚错误返回0；
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
 *@brief    读取某个引脚上的模拟电压
 *@param    pin：ADC1某通道映射的引脚
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压，默认参考电压为3.3V
            如果引脚错误返回0；
*/
uint16_t analog_read_voltage(Gpio *pin)
{
    return (uint16_t)(analog_read(pin) * 3300 / 4096); //求的结果扩大了1000倍，方便下面求出小数
}

/**
  * @}
  */
