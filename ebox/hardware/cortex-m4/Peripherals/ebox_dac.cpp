#include "ebox_dac.h"

static uint8_t dac_out_num = 0;

DACCLASS::DACCLASS(uint16_t dac_channel)
{
    this->dac_channel = dac_channel;
}

void DACCLASS::begin()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    DAC_InitTypeDef DAC_InitType;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);//使能GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);//使能DAC时钟
    switch(dac_channel)
    {
    case DAC1:
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
        GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

        DAC_InitType.DAC_Trigger = DAC_Trigger_None;	//不使用触发功能 TEN1=0
        DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; //不使用波形发生
        DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //屏蔽、幅值设置
        DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
        DAC_Init(DAC_Channel_1, &DAC_InitType);	 //初始化DAC通道1

        DAC_Cmd(DAC_Channel_1, ENABLE);  //使能DAC通道1
        DAC_SetChannel1Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
        break;
    }
    case DAC2:
    {
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;//模拟输入
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
        GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化

        DAC_InitType.DAC_Trigger = DAC_Trigger_None;	//不使用触发功能 TEN1=0
        DAC_InitType.DAC_WaveGeneration = DAC_WaveGeneration_None; //不使用波形发生
        DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0; //屏蔽、幅值设置
        DAC_InitType.DAC_OutputBuffer = DAC_OutputBuffer_Disable ;	//DAC1输出缓存关闭 BOFF1=1
        DAC_Init(DAC_Channel_2, &DAC_InitType);	 //初始化DAC通道1

        DAC_Cmd(DAC_Channel_2, ENABLE);  //使能DAC通道1
        DAC_SetChannel2Data(DAC_Align_12b_R, 0);  //12位右对齐数据格式设置DAC值
        break;
    }
    default :
        break;
    }
    dac_out_num = 0;
}

void DACCLASS::dac_out(int16_t value)
{
    switch(dac_channel)
    {
    case DAC1:
    {
        DAC_SetChannel1Data(DAC_Align_12b_R, value); //12位右对齐数据格式设置DAC值
        break;
    }
    case DAC2:
    {
        DAC_SetChannel2Data(DAC_Align_12b_R, value); //12位右对齐数据格式设置DAC值
        break;
    }
    default:
        break;
    }
}

void DACCLASS::dac_sin_out_update(void)
{
    dac_out((uint16_t)(Sin_tab[dac_out_num]));
    dac_out_num++;
    if(dac_out_num >= 32)
        dac_out_num = 0;

}
