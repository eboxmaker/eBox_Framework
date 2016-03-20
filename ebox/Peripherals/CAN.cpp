/*
file   : can.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "can.h"



const uint16_t CANBAUD[][4] =
{
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_3tq, 108 },     //33k
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_3tq, 72 },      //50k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 64 },      //62.5k
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_8tq, 25 },      //96k
    { CAN_SJW_1tq, CAN_BS1_6tq, CAN_BS2_3tq, 36 },      //100k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 32 },      //125k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 16 },      //250k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 8 },       //500k
    { CAN_SJW_1tq, CAN_BS1_9tq, CAN_BS2_5tq, 3 },       //800k
    { CAN_SJW_1tq, CAN_BS1_3tq, CAN_BS2_5tq, 4 },       //1M
};


CAN::CAN(CAN_TypeDef *CANx, GPIO *p_pin_rx, GPIO *p_pin_tx)
{
    _CANx = CANx;
    this->pin_rx = p_pin_rx;
    this->pin_tx = p_pin_tx;
}
void CAN::begin(BSP_CAN_BAUD bps)
{
    u8 i;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    pin_rx->mode(INPUT_PU);
    pin_tx->mode(AF_PP);

    set_bps(bps);

    CAN_ITConfig(_CANx, CAN_IT_FMP0, DISABLE);    //关闭FIFO0接收中断
    CAN_ITConfig(_CANx, CAN_IT_FMP1, DISABLE);    //打开FIFO1接收中断


    set_filter(CAN_FIFO0, CAN_ID_STD, 0, 1 << 5, 0xFFFFFFFF);
    set_filter(CAN_FIFO0, CAN_ID_STD, 1, 1 << 5, 0xFFFFFFFF);
    set_filter(CAN_FIFO1, CAN_ID_STD, 2, 1 << 5, 0xFFFFFFFF);

    for(i = 0; i < 3; i++)CAN_CancelTransmit(_CANx, i);
}



void CAN::set_bps(BSP_CAN_BAUD bps)
{
    CAN_InitTypeDef CAN_InitStructure;

    CAN_InitStructure.CAN_SJW = CANBAUD[bps][0];//CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CANBAUD[bps][1];//CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CANBAUD[bps][2];//CAN_BS2_8tq;
    CAN_InitStructure.CAN_Prescaler = CANBAUD[bps][3];//60;

    CAN_InitStructure.CAN_TTCM = DISABLE;   //禁用时间触发
    CAN_InitStructure.CAN_ABOM = ENABLE;    //自动离线管理
    CAN_InitStructure.CAN_AWUM = ENABLE;    //自动唤醒
    CAN_InitStructure.CAN_NART = ENABLE;    //错误不自动重传
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    CAN_Init(_CANx, &CAN_InitStructure);
}

void CAN::set_filter(u8 Fifo, u8 nCanType, u8 num, u32 ID, u32 Mask)
{
    CAN_FilterInitTypeDef CAN_FilterInitStructure;

    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterActivation = DISABLE;

    if(nCanType == CAN_ID_STD)
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_16bit;
    else
        CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;

    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = Fifo;
    CAN_FilterInitStructure.CAN_FilterNumber = num;
    CAN_FilterInitStructure.CAN_FilterIdHigh = ID >> 16;
    CAN_FilterInitStructure.CAN_FilterIdLow = ID & 0xffff;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = Mask >> 16;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = Mask & 0xffff;
    CAN_FilterInit(&CAN_FilterInitStructure );
}


u8 CAN::write(CanTxMsg *pCanMsg)
{
    u32 nTime = 0x3fff;
    u8 TMailbox;

    TMailbox = CAN_Transmit(_CANx, pCanMsg);

    while((CAN_TransmitStatus(_CANx, TMailbox) != CANTXOK) && (--nTime));

    if(nTime == 0)
    {
        CAN_CancelTransmit(_CANx, TMailbox);//发送错误时取消发送
        return false;
    }
    else
    {
        return true;
    }
}

u8 CAN::read(CanRxMsg *pCanMsg, u16 WaitTime)
{
    u32 ms;

    ms = millis();
    while(1)
    {
        if(CAN_MessagePending(_CANx, CAN_FIFO0) > 0)
        {
            CAN_Receive(_CANx, CAN_FIFO0, pCanMsg );
            return true;
        }

        if((ms + WaitTime) > millis())return false;
    }
}

callback_fun_type can_callback_table[2];

void CAN::attach_interrupt(void (*callback_fun)(void))
{
    switch((u32)_CANx)
    {
    case (u32)CAN1_BASE:
        can_callback_table[0] = callback_fun;
        break;
    case (u32)CAN2_BASE:
        can_callback_table[1] = callback_fun;
        break;
    }
}

void CAN::interrupt(FunctionalState enable)
{
    if(enable == ENABLE)
    {
        CAN_ITConfig(_CANx, CAN_IT_FMP1, ENABLE);
    }
    else
    {
        CAN_ITConfig(_CANx, CAN_IT_FMP1, DISABLE);
    }
}

#ifdef __cplusplus
extern "C" {
#endif

void CAN1_RX1_IRQHandler(void)
{
    can_callback_table[0]();
    CAN_FIFORelease(CAN1, CAN_FIFO1);
}

void CAN2_RX1_IRQHandler(void)
{
    can_callback_table[1]();
    CAN_FIFORelease(CAN2, CAN_FIFO1);
}

#ifdef __cplusplus
}
#endif

