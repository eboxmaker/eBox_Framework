#ifndef __DEBUG_H__
#define __DEBUG_H__
#include "ebox.h"

#define DEBUG
#ifdef DEBUG
#define DMSG(str)       uart1.print(str)
#define DMSG_HEX(n)     uart1.print(' '); uart1.print(n, HEX)
#else
#define DMSG(str)
#define DMSG_HEX(n)
#endif

#endif
