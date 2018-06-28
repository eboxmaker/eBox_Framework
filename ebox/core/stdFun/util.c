/**
  ******************************************************************************
  * @file    util.cpp
  * @author  shentq
  * @version V2.1
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
#include "util.h"

bool isdigital(char c)
{
    if(c <= '9' && c >= '0')
        return true;
    else 
        return false;

}





/**
@brief	replace the specified character in a string with new character
*/
void replacetochar(
    char *str, 		/**< pointer to be replaced */
    char oldchar, 	/**< old character */
    char newchar	/**< new character */
)
{
    int x;
    for (x = 0; str[x]; x++)
        if (str[x] == oldchar) str[x] = newchar;
}
/**
@brief	CONVERT CHAR INTO HEX
@return	HEX

This function converts HEX(0-F) to a character
*/
char C2D(
    uint8_t c	/**< is a character('0'-'F') to convert to HEX */
)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + c - 'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c - 'A';

    return (char)c;
}

uint16_t swaps(uint16_t i16)//交换高低字节
{
    uint16_t ret = 0;
    ret = (i16 & 0xFF) << 8;
    ret |= ((i16 >> 8) & 0xFF);
    return ret;
}

uint32_t swapl(uint32_t l32)//交换高低字节
{
    uint32_t ret = 0;
    ret = (l32 & 0xFF) << 24;
    ret |= ((l32 >> 8) & 0xFF) << 16;
    ret |= ((l32 >> 16) & 0xFF) << 8;
    ret |= ((l32 >> 24) & 0xFF);
    return ret;
}













