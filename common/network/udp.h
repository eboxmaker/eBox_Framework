/**
  ******************************************************************************
  * @file    udp.h
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

#ifndef __UDP_H
#define __UDP_H
#include "socket.h"
#include "w5500.h"


class UDPMessage
{
public:
    u8 *buf;
    u16 len;
    u8 *rIP;
    u16 rPort;
};

class UDP
{
public:
    u8 begin(SOCKET ps, uint16_t port);
    u16 send(UDPMessage *msg);
    u16 sendto(u8 *rIP, u16 rPort, u8 *buf, u16 len);
    u16 recv(u8 *buf);
    u8  close();

public:
    u8 remoteIP[4];/*配置服务器的IP地址*/
    u16 remotePort;/*定义计算机的一个端口并初始化*/
    u16 localPort;/*定义本地的一个端口并初始化*/
    SOCKET s;
    u8 recvFlag;


};

#endif
