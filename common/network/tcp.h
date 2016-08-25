/**
  ******************************************************************************
  * @file    tcp.h
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
#ifndef __TCP_H
#define __TCP_H
#include "socket.h"
#include "w5500.h"

class TCPCLIENT
{
public:
    int     begin(SOCKET ps, uint16_t port);
    bool	connect(uint8_t *IP, uint16_t Port);

    uint8_t     status();//网卡当前的状态
    uint16_t	available();//接收缓冲区是否可用
    bool	    connected();//是否存在链接
    void	    stop();

    uint16_t     recv(uint8_t *buf);
    uint16_t     recv(uint8_t *buf, uint16_t len);
    uint16_t 	send(uint8_t *buf, uint16_t len);

    uint16_t localPort;
    uint8_t  remoteIP[4];/*配置服务器的IP地址*/
    uint16_t remotePort;/*定义计算机的一个端口并初始化*/


private:
    SOCKET s;

};

class TCPSERVER
{
public:
    int begin(SOCKET ps, uint16_t port);
    uint16_t recv(uint8_t *buf);
    uint16_t send(uint8_t *buf, uint16_t len);
    void close();

    uint16_t localPort;
    uint8_t  remoteIP[4];/*配置服务器的IP地址*/
    uint16_t remotePort;/*定义计算机的一个端口并初始化*/
private:
    SOCKET s;



};
#endif
