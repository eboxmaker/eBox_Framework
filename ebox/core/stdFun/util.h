/**
  ******************************************************************************
  * @file    util.h
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
#ifndef _UTIL_H
#define _UTIL_H
#ifdef __cplusplus
extern "C"{
#endif // __cplusplus
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "ebox_type.h"

bool    isdigital(char c);
char    C2D(uint8_t c); 					/* Convert a character to HEX */



void    replacetochar(char *str, char oldchar, char newchar);


uint16_t    swaps(uint16_t i16);
uint32_t    swapl(uint32_t l32);

#ifdef __cplusplus
} 
#endif // __cplusplus

#endif
