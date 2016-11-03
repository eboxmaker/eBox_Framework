/**
  ******************************************************************************
  * @file    gp2y1051.cpp
  * @author  starli
  * @version V1.2
  * @date    2016/08/14
  * @brief   PM2.5´«¸ÐÆ÷Çý¶¯
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
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

        readnum = usart_port->read();

        if( readnum == 0xAA)
        {

            for(i = 0; i < 6; i++)
            {
                tmpbuf[i] = usart_port->read();
            }

            readnum = tmpbuf[0] + tmpbuf[1] + tmpbuf[2] + tmpbuf[3];
            if(( readnum == tmpbuf[4]) && ( 0xFF == tmpbuf[5] ))
            {
                *buf++ = (tmpbuf[0]);
                *buf++ = (tmpbuf[1]);
            }
        break;
        }

        uart1.printf("\r\voutH = %d", tmpbuf[0]);
        uart1.printf("\r\voutL = %d", tmpbuf[0]);
    }

}

