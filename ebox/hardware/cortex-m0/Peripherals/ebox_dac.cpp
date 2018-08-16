/*
file   : dac.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/
#include "ebox_dac.h"

uint32_t DualSine12bit[100];
#define DAC_DHR12RD_Address      0x40007420 //0x40007420

DACCLASS::DACCLASS(Gpio *pin)
{
    this->pin = pin;
}

void DACCLASS::begin(uint16_t *buf1, uint16_t *buf2, uint16_t buf_size)
{
    DAC_InitTypeDef  DAC_InitStructure;
    /* 使能DAC时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);


    pin->mode(AIN);

    /////////////////////////////////////////////////////////////////
    /* 配置DAC 通道1 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;						//使用TIM2作为触发源
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//不使用波形发生器
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//不使用DAC输出缓冲
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* 配置DAC 通道2 */
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* 使能通道1 由PA4输出 */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    /* 使能通道2 由PA5输出 */
    DAC_Cmd(DAC_Channel_2, ENABLE);

    /* 使能DAC的DMA请求 */
    DAC_DMACmd(DAC_Channel_2, ENABLE);

    /////////////////////////////////////////////////////////////////////////////////////////
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

    /* 使能TIM2时钟，TIM2CLK 为72M */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* TIM2基本定时器配置 */
    // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 19;       									//定时周期 20
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;       							//预分频，不分频 72M / (0+1) = 72M
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//时钟分频系数
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//向上计数模式
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* 配置TIM2触发源 */
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

    /* 使能TIM2 */
    TIM_Cmd(TIM2, ENABLE);

    /////////////////////////////////////////////////////////////////////////////////////////
    DMA_InitTypeDef  DMA_InitStructure;

    /* 使能DMA2时钟 */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    /* 配置DMA2 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12RD;					//外设数据地址
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DualSine12bit ;				//内存数据地址 DualSine12bit
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//数据传输方向内存至外设
    DMA_InitStructure.DMA_BufferSize = 100;																	//缓存大小为32字节
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//外设数据地址固定
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//内存数据地址自增
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//外设数据以字为单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//内存数据以字为单位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//循环模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//高DMA通道优先级
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//非内存至内存模式

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /* 使能DMA2-14通道 */
    DMA_Cmd(DMA2_Channel4, ENABLE);

    for (int Idx = 0; Idx < buf_size; Idx++)
    {
        DualSine12bit[Idx] = (buf1[Idx] << 16) + (buf2[Idx]);
    }

}
