/**
  ******************************************************************************
  * @file    netconfig.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * Copyright 2015 shentq. All Rights Reserved.
  *
  * Copyright Notice
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq.
  *
  * @Disclaimer
  * This specification is preliminary and is subject to change at any time without
  * notice. shentq assumes no responsibility for any errors contained herein.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __NETCONFIG_H
#define __NETCONFIG_H
#include "stm32f10x.h"

class IPCONFIG
{
    public:
        uint8_t mac[6];
        uint8_t ip[4];
        uint8_t subnet[4];
        uint8_t gw[4];
        uint8_t dns[4];    
};

#endif
