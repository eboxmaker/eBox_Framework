/***************************************************************************************************************
Copyright (c) 2005 Embedded Internet Solutions, Inc
All rights reserved. You are not allowed to copy or distribute
the code without permission.
FileName: malloc.c
Author: 何飞                              Version : 1.0                              Date:2009-3-18
Description: 模拟操作系统的格式输出
Version: 	无
Function List: printf	格式输出到终端显示
			vsprintf	格式输出到buf
**************************************************************************************************************/
#ifndef _MY_PRINTF_H
#define _MY_PRINTF_H

#include <stdlib.h>
#include  <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif



int _ebox_vsnprintf(char *buf, size_t size, const char *format, va_list args);
int _ebox_snprintf(char *str, size_t size, const char *format, ...);
int _ebox_vsprintf(char *ret, const char *format, va_list ap);
int _ebox_sprintf(char *str, const char *format, ...);

#ifdef __cplusplus
}
#endif
#endif



