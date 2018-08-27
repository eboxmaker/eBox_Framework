#ifndef __EBOX_SNPRINTF_H
#define __EBOX_SNPRINTF_H

#include <stdarg.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif
extern int
rpl_vsnprintf(char *str, size_t size, const char *format, va_list args);
extern int
rpl_snprintf(char *str, size_t size, const char *format, ...);

int rpl_vsprintf(char *str, const char *format, ...);

extern int
rpl_sprintf(char *str, const char *format, ...);

#ifdef __cplusplus
}
#endif

#endif
