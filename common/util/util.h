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

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>



#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))



uint16_t    ATOI(char *str, uint16_t base); 			/* Convert a string to integer number */
uint32_t    ATOI32(char *str, uint16_t base); 			/* Convert a string to integer number */
void        itoa(uint16_t n, uint8_t *str, uint8_t len);
int         ValidATOI(char *str, int base, int *ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char        C2D(uint8_t c); 					/* Convert a character to HEX */

uint16_t    swaps(uint16_t i16);
uint32_t    swapl(uint32_t l32);

void        replacetochar(char *str, char oldchar, char newchar);

void        inet_addr_(unsigned char *addr, unsigned char *ip);

int         find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek);
uint16_t    get_str(char *source, const char *begin, uint16_t count1, const char *end, uint16_t count2, char *out);
uint16_t    get_str(char *source, const char *begin, uint16_t count, uint16_t length, char *out);
uint16_t    get_str(char *source, char *out, uint16_t length);
#endif
