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
    uint8_t *buf;
    uint16_t len;
    uint8_t *rIP;
    uint16_t rPort;
};

class UDP
{
public:
    uint8_t begin(SOCKET ps, uint16_t port);
    uint16_t send(UDPMessage *msg);
    uint16_t sendto(uint8_t *rIP, uint16_t rPort, uint8_t *buf, uint16_t len);
    uint16_t recv(uint8_t *buf);
    uint8_t  close();

public:
    uint8_t remoteIP[4];/*配置服务器的IP地址*/
    uint16_t remotePort;/*定义计算机的一个端口并初始化*/
    uint16_t localPort;/*定义本地的一个端口并初始化*/
    SOCKET s;
    uint8_t recvFlag;


};

#endif
