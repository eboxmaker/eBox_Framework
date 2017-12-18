/**
  ******************************************************************************
  * @file    gp2y1051.h
  * @author  shentq
  * @version V1.2
  * @date    2016/08/14
  * @brief   
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

/* Define to prevent recursive inclusion -------------------------------------*/


#ifndef  __GP2Y1051_H
#define  __GP2Y1051_H

#include "ebox.h"
#include "ebox_uart.h"



class GP2Y1051
{
public:

    GP2Y1051(Uart *p_usart_port)
    {
        usart_port = p_usart_port;
    };


    void begin();
    void get_data(int8_t *buf);

private:
    Uart *usart_port;
};



#endif
