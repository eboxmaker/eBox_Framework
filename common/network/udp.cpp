/**
  ******************************************************************************
  * @file    udp.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "udp.h"

u8 UDP::begin(SOCKET ps, uint16_t port)
{
    u8 ret;
    s = ps;
    localPort = port;
    ret = _socket(s, Sn_MR_UDP, localPort, 0); /*初始化socket 0的套接字*/
    return ret;

}
u16 UDP::send(UDPMessage *msg)
{
    return _sendto(s, msg->buf, msg->len, msg->rIP, msg->rPort);
}
u16 UDP::sendto(u8 *rIP, u16 rPort, u8 *buf, u16 len)
{
    len = _sendto(s, buf, len, rIP, rPort);
    return len;
}
u16 UDP::recv(u8 *buf)
{
    u16 len = 0;
    if((len = recv_available(s)) > 0)
    {
        len = _recvfrom(s, buf, len, remoteIP, &remotePort); /*W5500接收计算机发送来的数据*/
        recvFlag = 0;
    }
    return len;
}
u8 UDP::close()
{
    _close(s);
    return 0;
}

