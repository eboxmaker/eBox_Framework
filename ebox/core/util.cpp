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




/**
@brief	CONVERT STRING INTO INTEGER
@return	a integer number
*/

bool isspace(char c)
{
    if(c == ' ')
        return true;
    else
        return false;
}
bool isdigital(char c)
{
    if(c <= '9' && c >= '0')
        return true;
    else 
        return false;

}
int atoi(
    char *str,	/**< is a pointer to convert */
    uint16_t base	/**< is a base value (must be in the range 2 - 16) */
)
{
    
    int c;              /* current char */  
    int total = 0;         /* current total */  
    int sign;           /* if '-', then negative, otherwise positive */  

    /* skip whitespace */  
    while ( isspace((char)*str) )  
            str++;  
    
    c = (int)(unsigned char)*str;  
    sign = c;           /* save sign indication */  
    if (c == '-' || c == '+')  
        c = (int)(unsigned char)*str++;    /* skip sign */  
        
    while (*str != 0)
        total = total * base + C2D(*str++);
    if(sign == '-')
        return -total;
    else
        return total;
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

    *ret = atoi(str, base);
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
        return 10 + c - 'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c - 'A';

    return (char)c;
}

//void swap(float *a, float *b) 
//{
//	float temp = *a;
//	*a = *b;
//	*b = temp;
//}
//void swap(int *a, int *b) 
//{
//	int temp = *a;
//	*a = *b;
//	*b = temp;
//}
uint16_t swaps(uint16_t i16)//�����ߵ��ֽ�
{
    uint16_t ret = 0;
    ret = (i16 & 0xFF) << 8;
    ret |= ((i16 >> 8) & 0xFF);
    return ret;
}

uint32_t swapl(uint32_t l32)//�����ߵ��ֽ�
{
    uint32_t ret = 0;
    ret = (l32 & 0xFF) << 24;
    ret |= ((l32 >> 8) & 0xFF) << 16;
    ret |= ((l32 >> 16) & 0xFF) << 8;
    ret |= ((l32 >> 24) & 0xFF);
    return ret;
}

int find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek)
{
    uint16_t _count = 1;
    uint16_t len = 0;
    seek = 0;
    uint8_t *temp_str = NULL;
    uint8_t *temp_ptr = NULL;
    uint8_t *temp_char = NULL;
    if(0 == s_str || 0 == p_str)
        return 0;
    for(temp_str = s_str; *temp_str != '\0'; temp_str++)	 //���β����ַ���
    {
        temp_char = temp_str; //ָ��ǰ�ַ���
        //�Ƚ�
        for(temp_ptr = p_str; *temp_ptr != '\0'; temp_ptr++)
        {
            if(*temp_ptr != *temp_char)
            {
                len = 0;
                break;
            }
            temp_char++;
            len++;
        }
        if(*temp_ptr == '\0') //��������Ҫ���ҵ��ַ����˳�
        {
            if(_count == count)
                return len;
            else
            {
                _count++;
                len = 0;
            }
        }
        seek++;  //��ǰƫ������1
    }
    return 0;
}

//�����ַ���
//���������ַ����м���ַ�����count1,count2�ֱ��������ַ������ֵĵڼ���
uint16_t get_str(char *source, const char *begin, uint16_t count1, const char *end, uint16_t count2, char *out)
{
    uint16_t i;
    uint16_t len1;
    uint16_t len2;
    uint16_t index1 = 0;
    uint16_t index2 = 0;
    uint16_t length = 0;
    len1 = find_str((uint8_t *)source, (uint8_t *)begin, count1, index1);
    len2 = find_str((uint8_t *)source, (uint8_t *)end, count2, index2);
    length = index2 - index1 - len1;
    if((len1 != 0) && (len2 != 0))
    {
        for( i = 0; i < index2 - index1 - len1; i++)
            out[i] = source[index1 + len1 + i];
        out[i] = '\0';
    }
    return length;

}
//�����ַ���
//�����ڵ�count��begin�ַ���֮��length�����ַ���
uint16_t get_str(char *source, const char *begin, uint16_t count, uint16_t length, char *out)
{
    uint16_t i = 0;
    uint16_t len1 = 0;
    uint16_t index1 = 0;
    len1 = find_str((uint8_t *)source, (uint8_t *)begin, count, index1);
    if(len1 != 0)
    {
        for(i = 0; i < length; i++)
            out[i] = source[index1 + len1 + i];
        out[i] = '\0';
    }
    return length;
}
//�����ַ���
//���Ҵӿ�ʼ��length�м���ַ���
uint16_t get_str(char *source, char *out, uint16_t length)
{
    uint16_t i = 0;
    for (i = 0 ; i < length ; i++)
    {
        out[i] = source[i];
    }
    out[i] = '\0';
    return length;
}
//�����ַ���
//���Ҵӿ�ʼ����count��end֮����ַ���
uint16_t get_str(char *source, const char *end, uint16_t count, char *out)
{
    uint16_t i;
    uint16_t len;
    uint16_t index = 0;
    uint16_t length = 0;
    len = find_str((uint8_t *)source, (uint8_t *)end, count, index);
    length = index;
    if((len != 0) && (len != 0))
    {
        for( i = 0; i < length; i++)
            out[i] = source[i];
        out[i] = '\0';
    }
    return length;
}
float max(float *p,uint16_t len)
{
    
return 0;
}


int partion(int arr[], int low, int high) {
	int i = low-1;
	int j = low;

	int x = arr[high];

	for (; j<high; j++) {
		if (arr[j] < x)
			swap(&arr[++i], &arr[j]);

	}
	swap(&arr[i+1], &arr[high]);

	return i+1;
}
void quick_sort(int arr[], int low, int high) {
	if (low >= high)
		return;
	int mid = partion(arr, low, high);
	quick_sort(arr, low, mid-1);
	quick_sort(arr, mid+1, high);
}
int partion(float arr[], int low, int high) {
	int i = low-1;
	int j = low;

	int x = arr[high];

	for (; j<high; j++) {
		if (arr[j] < x)
			swap(&arr[++i], &arr[j]);

	}
	swap(&arr[i+1], &arr[high]);

	return i+1;
}
void quick_sort(float arr[], int low, int high) {
	if (low >= high)
		return;
	int mid = partion(arr, low, high);
	quick_sort(arr, low, mid-1);
	quick_sort(arr, mid+1, high);
}