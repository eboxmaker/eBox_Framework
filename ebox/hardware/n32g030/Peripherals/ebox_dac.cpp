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
    /* ʹ��DACʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);


    pin->mode(AIN);

    /////////////////////////////////////////////////////////////////
    /* ����DAC ͨ��1 */
    DAC_InitStructure.DAC_Trigger = DAC_Trigger_T2_TRGO;						//ʹ��TIM2��Ϊ����Դ
    DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;	//��ʹ�ò��η�����
    DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;	//��ʹ��DAC�������
    DAC_Init(DAC_Channel_1, &DAC_InitStructure);

    /* ����DAC ͨ��2 */
    DAC_Init(DAC_Channel_2, &DAC_InitStructure);

    /* ʹ��ͨ��1 ��PA4��� */
    DAC_Cmd(DAC_Channel_1, ENABLE);
    /* ʹ��ͨ��2 ��PA5��� */
    DAC_Cmd(DAC_Channel_2, ENABLE);

    /* ʹ��DAC��DMA���� */
    DAC_DMACmd(DAC_Channel_2, ENABLE);

    /////////////////////////////////////////////////////////////////////////////////////////
    TIM_TimeBaseInitTypeDef    TIM_TimeBaseStructure;

    /* ʹ��TIM2ʱ�ӣ�TIM2CLK Ϊ72M */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    /* TIM2������ʱ������ */
    // TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
    TIM_TimeBaseStructure.TIM_Period = 19;       									//��ʱ���� 20
    TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;       							//Ԥ��Ƶ������Ƶ 72M / (0+1) = 72M
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    						//ʱ�ӷ�Ƶϵ��
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//���ϼ���ģʽ
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    /* ����TIM2����Դ */
    TIM_SelectOutputTrigger(TIM2, TIM_TRGOSource_Update);

    /* ʹ��TIM2 */
    TIM_Cmd(TIM2, ENABLE);

    /////////////////////////////////////////////////////////////////////////////////////////
    DMA_InitTypeDef  DMA_InitStructure;

    /* ʹ��DMA2ʱ�� */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);

    /* ����DMA2 */
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&DAC->DHR12RD;					//�������ݵ�ַ
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)DualSine12bit ;				//�ڴ����ݵ�ַ DualSine12bit
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;											//���ݴ��䷽���ڴ�������
    DMA_InitStructure.DMA_BufferSize = 100;																	//�����СΪ32�ֽ�
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;				//�������ݵ�ַ�̶�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;									//�ڴ����ݵ�ַ����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;	//������������Ϊ��λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;					//�ڴ���������Ϊ��λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;													//ѭ��ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;											//��DMAͨ�����ȼ�
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;														//���ڴ����ڴ�ģʽ

    DMA_Init(DMA2_Channel4, &DMA_InitStructure);

    /* ʹ��DMA2-14ͨ�� */
    DMA_Cmd(DMA2_Channel4, ENABLE);

    for (int Idx = 0; Idx < buf_size; Idx++)
    {
        DualSine12bit[Idx] = (buf1[Idx] << 16) + (buf2[Idx]);
    }

}
