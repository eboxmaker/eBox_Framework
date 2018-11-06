
#include "ebox_adc.h"


//默认开启16通道 采用DMA+ADC连续转换模式。提供AD采集服务
//只需将IO设置为AIN模式即可读取引脚相应的电压
#define CH 16
u16  AD_value[CH];   //用来存放ADC转换结果，也是DMA的目标地址


/**
 *@name     void ADC1_configuration(void)
 *@brief    ADC配置，将ADC1配置为16通道规则采样，采样周期28.5个周期，10bit精度
 *@param    NONE
 *@retval   NONE
*/
void ADC1_configuration(void)
{

    ADC_InitTypeDef  ADC_InitStructure;
    ADC_CommonInitTypeDef ADC_CommonInitStructure;

    ADC_DeInit();  //将外设 ADC1 的全部寄存器重设为缺省值

    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);	 //ADC1复位
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);	//复位结束

    /* ADC1 configuration ------------------------------------------------------*/

    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;//两个采样阶段之间的延迟5个时钟
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;//ADC_DMAAccessMode_Disabled; //DMA失能
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;//预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
    ADC_CommonInit(&ADC_CommonInitStructure);//初始化

    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;//DISABLE;//非扫描模式
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//DISABLE;//关闭连续转换
    ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
    ADC_InitStructure.ADC_NbrOfConversion = CH;//1;//1个转换在规则序列中 也就是只转换规则序列1
    ADC_Init(ADC1, &ADC_InitStructure);//ADC初始化

    /* ADC1 regular channel11 configuration */
    //设置指定ADC的规则组通道，设置它们的转化顺序和采样时间
    //ADC1,ADC通道x,规则采样顺序值为y,采样时间为239.5周期

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

    // 开启ADC的DMA支持（要实现DMA功能，还需独立配置DMA通道等参数）

    ADC_DMARequestAfterLastTransferCmd(ADC1, ENABLE);
    ADC_DMACmd(ADC1, ENABLE);

    /* Enable ADC1 */
    ADC_Cmd(ADC1, ENABLE);	   //使能指定的ADC1
    /* Enable ADC1 reset calibaration register */
    // ADC_ResetCalibration(ADC1);	  //复位指定的ADC1的校准寄存器
    /* Check the end of ADC1 reset calibration register */
    // while(ADC_GetResetCalibrationStatus(ADC1));	//获取ADC1复位校准寄存器的状态,设置状态则等待

    /* Start ADC1 calibaration */
    // ADC_StartCalibration(ADC1);		//开始指定ADC1的校准状态
    /* Check the end of ADC1 calibration */
    //  while(ADC_GetCalibrationStatus(ADC1));		//获取指定ADC1的校准程序,设置状态则等待

}
/**
 *@name     void DMA_configuration(void)
 *@brief    ADC的DMA配置，将ADC1配置为DMA自动传输模式。转换结果自动的传输到内存
 *@param    NONE
 *@retval   NONE
*/
void DMA_configuration(void)
{
    /* ADC1  DMA1 Channel Config */
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(DMA2_Stream0);   //将DMA的通道1寄存器重设为缺省值
    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE) {} //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;//通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR; //DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr =  (uint32_t)&AD_value;//DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = CH;//数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
    DMA_InitStructure.DMA_MemoryInc =  DMA_MemoryInc_Enable;//存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;//外设数据长度:16位
    DMA_InitStructure.DMA_MemoryDataSize =     DMA_MemoryDataSize_HalfWord;//存储器数据长度:16位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;// 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;//高等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);//初始化DMA Stream

    DMA_Cmd(DMA2_Stream0, ENABLE);

}

/**
 *@name     void ADC1_init(void)
 *@brief    ADC初始化
 *@param    NONE
 *@retval   NONE
*/
void ADC1_init(void)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE );	   //使能ADC1通道时钟，各个管脚时钟

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); //DMA1时钟使能

    ADC1_configuration();
    DMA_configuration();
    ADC_SoftwareStartConv(ADC1);

}
/**
 *@name     uint16_t analog_read(GPIO *pin)
 *@brief    读取ADC1某个引脚上的模拟转换结果
 *@param    pin：ADC1某通道映射的引脚
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的10bit的ADC转换结果
            如果引脚错误返回0；
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
 *@brief    读取某个引脚上的模拟电压
 *@param    pin：ADC1某通道映射的引脚
 *@retval   如果引脚正确则返回该引脚的模拟电压值所对应的模拟电压，默认参考电压为3.3V
            如果引脚错误返回0；
*/
uint16_t analog_read_voltage(Gpio *pin)
{
    return (u16)(analog_read(pin) * 3300 / 4096); //求的结果扩大了1000倍，方便下面求出小数
}








