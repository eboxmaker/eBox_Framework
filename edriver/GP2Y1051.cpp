/*

//	PM2.5´«¸ÐÆ÷Çý¶¯
file   : GP2Y1051.cpp
author : starli
version: V1.0
date   : 2015/7/5


*/

#include "GP2Y1051.h"


void GP2Y1051::begin()
{
    usart_port->begin(2400);
}

void GP2Y1051::get_data(int8_t *buf)
{
    //
    uint8_t i, readnum;
    uint8_t tmpbuf[10];

    while(1)
    {

        readnum = usart_port->receive();

        if( readnum == 0xAA)
        {

            for(i = 0; i < 6; i++)
            {
                tmpbuf[i] = usart_port->receive();
            }

            readnum = tmpbuf[0] + tmpbuf[1] + tmpbuf[2] + tmpbuf[3];
            if(( readnum == tmpbuf[4]) && ( 0xFF == tmpbuf[5] ))
            {
                *buf++ = (tmpbuf[0]);
                *buf++ = (tmpbuf[1]);
            }
        }
        break;

        uart1.printf("\r\voutH = %d", tmpbuf[0]);
        uart1.printf("\r\voutL = %d", tmpbuf[0]);
    }

}

