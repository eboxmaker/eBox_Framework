/*

//	PM2.5´«¸ÐÆ÷Çý¶¯
file   : GP2Y1051.cpp
author : starli
version: V1.0
date   : 2015/7/5


*/

#ifndef  __GP2Y1051_H
#define  __GP2Y1051_H

#include "ebox.h"
#include "uart.h"



class GP2Y1051
{
public:

    GP2Y1051(USART *p_usart_port)
    {
        usart_port = p_usart_port;
    };


    void begin();
    void get_data(int8_t *buf);

private:
    USART *usart_port;
};



#endif
