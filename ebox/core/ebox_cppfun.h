#ifndef __EBOX_CPPFUN_H
#define __EBOX_CPPFUN_H


#include "../core/stdFun/new.h"
#include "../core/stdFun/print.h"
#include "../core/stdFun/stream.h"
#include "../core/stdFun/ringbuf.h"
#include "../core/stdFun/list.h"
#include "../core/stdFun/random.h"
#include "../core/stdFun/sort.h"
#include "../core/stdFun/Dstring.h"
#include "../core/stdFun/Wstring.h"
#include "../core/stdFun/IPAddress.h"
#include "../core/stdFun/Udp.h"
#include "../core/stdFun/client.h"
#include "../core/stdFun/server.h"




//限制某个数的下界
template<typename T>
void limit_low(T &num, T limL)
{
	if (num < limL)
	{
		num = limL;
	}
}

//限制某个数的上界
template<typename T>
void limit_high(T &num, T limH)
{
	if (num > limH)
	{
		num = limH;
	}
}


//限制某个数的上下界
template<typename T>
void limit(T &num, T limL, T limH)
{
	limit_low(num, limL);
	limit_high(num, limH);
}

#endif
