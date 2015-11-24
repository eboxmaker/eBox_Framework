/*
*
@file		util.h
@brief	
*/

#ifndef _UTIL_H
#define _UTIL_H

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdint.h>



#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))




uint16_t ATOI(char* str,uint16_t base); 			/* Convert a string to integer number */
uint32_t ATOI32(char* str,uint16_t base); 			/* Convert a string to integer number */
void itoa(uint16_t n,uint8_t* str, uint8_t len);
int ValidATOI(char* str, int base, int* ret); 		/* Verify character string and Convert it to (hexa-)decimal. */
char C2D(uint8_t c); 					/* Convert a character to HEX */

uint16_t swaps(uint16_t i16);
uint32_t swapl(uint32_t l32);

void replacetochar(char * str, char oldchar, char newchar);

void mid(int8_t* src, int8_t* s1, int8_t* s2, int8_t* sub);
void inet_addr_(unsigned char* addr,unsigned char *ip);
int find_str(uint8_t *s_str,uint8_t *p_str,uint16_t &seek);
#endif
