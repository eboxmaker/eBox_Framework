/*
file   : util.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "util.h"
#include "ebox.h"




/**
@brief	CONVERT STRING INTO INTEGER
@return	a integer number
*/
uint16_t ATOI(
	char *str,	/**< is a pointer to convert */
	uint16_t base	/**< is a base value (must be in the range 2 - 16) */
	)
{
  unsigned int num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}

uint32_t ATOI32(
	char *str,	/**< is a pointer to convert */
	uint16_t base	/**< is a base value (must be in the range 2 - 16) */
	)
{
  uint32_t num = 0;
  while (*str !=0)
          num = num * base + C2D(*str++);
  return num;
}


void itoa(uint16_t n,uint8_t str[5], uint8_t len)
{
  
  uint8_t i=len-1;

  memset(str,0x20,len);
  do{
  str[i--]=n%10+'0';
  
 }while((n/=10)>0);

 return;
}
/**
@brief	CONVERT STRING INTO HEX OR DECIMAL
@return	success - 1, fail - 0
*/
int ValidATOI(
	char *str, 	/**< is a pointer to string to be converted */
	int base, 	/**< is a base value (must be in the range 2 - 16) */
	int *ret		/**<  is a integer pointer to return */
	)
{
  int c;
  char *tstr = str;
  if(str == 0 || *str == '\0') return 0;
  while(*tstr != '\0')
  {
    c = C2D(*tstr);
    if( c >= 0 && c < base) tstr++;
    else    return 0;
  }
  
  *ret = ATOI(str,base);
  return 1;
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
		return 10 + c -'a';
	if (c >= 'A' && c <= 'F')
		return 10 + c -'A';

	return (char)c;
}

uint16_t swaps(uint16_t i16)//交换高低字节
{
  uint16_t ret=0;
  ret = (i16 & 0xFF) << 8;
  ret |= ((i16 >> 8)& 0xFF);
  return ret;	
}

uint32_t swapl(uint32_t l32)//交换高低字节
{
  uint32_t ret=0;
  ret = (l32 & 0xFF) << 24;
  ret |= ((l32 >> 8) & 0xFF) << 16;
  ret |= ((l32 >> 16) & 0xFF) << 8;
  ret |= ((l32 >> 24) & 0xFF);
  return ret;
}


void inet_addr_(unsigned char* addr,unsigned char *ip)
{
	int i;
//	u_long inetaddr = 0;
	char taddr[30];
	char *nexttok;
	char num;
	strcpy(taddr,(char *)addr);
	
	nexttok = taddr;
	for(i = 0; i < 4 ; i++)
	{
		nexttok = strtok(nexttok,".");
		if(nexttok[0] == '0' && nexttok[1] == 'x') num = ATOI(nexttok+2,0x10);
		else num = ATOI(nexttok,10);
		
		ip[i] = num;
		nexttok = NULL;
	}
}	
int find_str(uint8_t *s_str,uint8_t *p_str,uint16_t &seek)
{
	seek = 0;
	uint8_t *temp_str=NULL;
	uint8_t *temp_ptr=NULL;
	uint8_t *temp_char=NULL;
	if(0==s_str||0==p_str)
		return -2;
	for(temp_str=s_str;*temp_str!='\0';temp_str++)	 //依次查找字符串
	{
		temp_char=temp_str; //指向当前字符串
		//比较
		for(temp_ptr=p_str;*temp_ptr!='\0';temp_ptr++)
		{	
			if(*temp_ptr!=*temp_char)
			break;
			temp_char++;
		}
		if(*temp_ptr=='\0')  //出现了所要查找的字符，退出
		{
			return 0;
		}
		seek++;  //当前偏移量加1
	}
	return -1;
}



