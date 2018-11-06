/**
  ******************************************************************************
  * @file    wdg.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   2018/10/15 ���ò��������ָ������
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
#include "ebox_wdg.h"

const uint16_t PR[7] = {4, 8, 16, 32, 64, 128, 256};

void Iwdg::begin(uint16_t ms)
{
    uint8_t pr  = 0;	//��Ƶ�� 0-7
    uint16_t rlr = 0;	//���ؼĴ���ֵ��12λ��Чλ�����ֵ4095
    // ��������ֵ
    for (pr = 0; pr < 7; pr++)
    {
        rlr = ms * 40 / PR[pr];
        if (rlr <= 0x0fff) break;
    }
    // �������������ֵ����0xfffʱ��ȡ���ֵrlr = 4095, pr = 6;
    // ��ʱ��ʱTout=(256*4095)/40 (ms)
    if ( rlr > 0x0fff || pr >= 7 )
    {
        pr = 6;
        rlr = 0xfff;
    }

    IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
    IWDG_SetPrescaler(pr);
    IWDG_SetReload(rlr);
    IWDG_ReloadCounter();
    IWDG_Enable();
}
void Iwdg::feed()
{
    IWDG_ReloadCounter();    /*reload*/
}
