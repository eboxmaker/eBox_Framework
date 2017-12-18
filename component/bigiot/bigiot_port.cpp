/**
  ******************************************************************************
  * @file    bigiot_port.cpp
  * @author  shentq
  * @version V0.1
  * @date    2016/10/26
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


/* Includes ------------------------------------------------------------------*/
#include "bigiot_port.h"


bool BigIotPort::connect(const char *remote_ip, uint32_t remote_port, uint16_t local_port)
{
    return WIFI_TCP::connect((char *)remote_ip,remote_port,local_port);
}
bool BigIotPort::connected()
{
    return WIFI_TCP::connected();
}
uint16_t BigIotPort::available()
{
    return WIFI_TCP::available();
}

uint16_t BigIotPort::read_until(uint8_t *buf,char ch)
{
    uint16_t len=0;
    if(available()){
        len = WIFI_TCP::read_until(buf,ch);
    }
    return len;
}
uint16_t BigIotPort::send(uint8_t *buf,uint16_t length)
{
    return WIFI_TCP::send(buf,length);

}
