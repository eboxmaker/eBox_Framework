#ifndef __EBOX_PRINTF_H
#define __EBOX_PRINTF_H

#include "ebox_core.h"


#if USE_PRINTF

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

#include "fifo.h"

    
extern struct ebox_fifo *uart_fifo_ptr;

void ebox_printf_init(void);

void ebox_printf(const char *fmt, ...);
    
#ifdef __cplusplus
}
#endif
#endif

#endif
