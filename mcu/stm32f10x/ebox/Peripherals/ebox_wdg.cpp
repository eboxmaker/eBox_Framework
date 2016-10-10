/**
  ******************************************************************************
  * @file    wdg.cpp
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
#include "ebox_wdg.h"
#include "math.h"
void Iwdg::begin(uint16_t ms)
{

    uint8_t pr;
    uint16_t rlr;

    for(pr = 1; pr < 6; pr++)
    {
        rlr = ms * 40 / (4 * pow(2.0, pr));
        if(rlr <= 0x0fff) break;
    }
    if(pr == 5 || rlr > 0x0fff)
    {
        pr = 5;
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
