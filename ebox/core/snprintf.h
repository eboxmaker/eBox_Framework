#ifndef __EBOX_SNPRINTF_H
#define __EBOX_SNPRINTF_H
#include "stdarg.h"
#include "stdio.h"

#ifdef __cplusplus
extern "C" {
#endif
extern int
rpl_vsnprintf(char *str, size_t size, const char *format, va_list args);

#ifdef __cplusplus
}
#endif

#endif
