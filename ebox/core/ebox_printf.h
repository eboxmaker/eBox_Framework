#ifndef __EBOX_PRINTF_H
#define __EBOX_PRINTF_H

#include <stdio.h>
#include <stdarg.h>
#include "ebox_core.h"
#include "fifo.h"


#if USE_PRINTF

#ifdef __cplusplus
extern "C" {
#endif


size_t ebox_printf(const char *fmt, ...);
      void hard_fault_isr();

#ifdef __cplusplus
}
#endif
#endif

#endif
