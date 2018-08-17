/***************************************************************************************************************
Copyright (c) 2005 Embedded Internet Solutions, Inc
All rights reserved. You are not allowed to copy or distribute
the code without permission.
FileName: malloc.c
Author: �η�                              Version : 1.0                              Date:2009-3-18
Description: ģ�����ϵͳ�ĸ�ʽ���
Version: 	��
Function List: printf	��ʽ������ն���ʾ
			vsprintf	��ʽ�����buf			
**************************************************************************************************************/
#ifndef _MY_PRINTF_H
#define _MY_PRINTF_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdlib.h"
#include  "stdarg.h"
    
//int MyVsprintf(char *buf,const char *format,va_list args);
//extern char * MyPrintf(const char *format,...);//��ʽ������ն�
extern int Str2Digital(const char **str);
extern int Int2Str(int num , char *str);
//extern char * Int2Str(int num);
extern void ReverseStr(char *str);
extern void Convert(int num,int n , char *str);
extern int Float2Str(double num,char *str,int precision);//����ʵ�ʵ�precision
extern int mystrlen(char *str);
unsigned long long mypow10(int y);
int get_float_integer_len(float value);

//extern void vsn_test(const char *fmt, ...);
    
int _ebox_vsnprintf(char *buf,size_t size,const char *format,va_list args);
int _ebox_snprintf(char *str, size_t size,const char *format,...);
int _ebox_vsprintf(char *ret, const char *format, va_list ap);
int _ebox_sprintf(char *str,const char *format,...);

#ifdef __cplusplus
}
#endif
#endif



