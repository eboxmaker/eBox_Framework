/**
  ******************************************************************************
  * @file    can.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __CAN_H
#define __CAN_H

#include "common.h"

typedef enum
{
    BSP_CAN_33KBPS = 0,              // ������ 33K ģʽ
    BSP_CAN_50KBPS,                  // ������ 50K ģʽ
    BSP_CAN_625KBPS,                 // ������ 62.5K ģʽ
    BSP_CAN_96KBPS,                  // ������ 96K ģʽ
    BSP_CAN_100KBPS,                 // ������ 100K ģʽ
    BSP_CAN_125KBPS,                 // ������ 125K ģʽ
    BSP_CAN_250KBPS,                 // ������ 250K ģʽ
    BSP_CAN_500KBPS,                 // ������ 500K ģʽ
    BSP_CAN_800KBPS,                 // ������ 800K ģʽ
    BSP_CAN_1MBPS,                   // ������ 1Mģʽ
} BSP_CAN_BAUD;

class CAN
{
public:
    CAN(CAN_TypeDef *CANx, Gpio *p_pin_rx, Gpio *p_pin_tx);
    void begin(BSP_CAN_BAUD bps);
    void set_filter(u8 Fifo, u8 nCanType, u8 num, u32 ID, u32 Mask);
    void interrupt(FunctionalState enable);
    void attach_interrupt(void (*callback_fun)(void));

    u8   write(CanTxMsg *pCanMsg);
    u8   read(CanRxMsg *pCanMsg, u16 WaitTime);

private:
    void set_bps(BSP_CAN_BAUD);

private:
    CAN_TypeDef *_CANx;
    Gpio *pin_rx;           //arduino pin number
    Gpio *pin_tx;           //arduino pin number

    BSP_CAN_BAUD _bps;
};

#endif
