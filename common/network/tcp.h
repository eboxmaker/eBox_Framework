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
    bool	connect(u8 *IP, uint16_t Port);

    uint8_t     status();//������ǰ��״̬
    uint16_t	available();//���ջ������Ƿ����
    bool	    connected();//�Ƿ��������
    void	    stop();

    u16     recv(u8 *buf);
    u16     recv(u8 *buf, uint16_t len);
    u16 	send(u8 *buf, u16 len);

    u16 localPort;
    u8  remoteIP[4];/*���÷�������IP��ַ*/
    u16 remotePort;/*����������һ���˿ڲ���ʼ��*/


private:
    SOCKET s;

};

class TCPSERVER
{
public:
    int begin(SOCKET ps, uint16_t port);
    u16 recv(u8 *buf);
    u16 send(u8 *buf, u16 len);
    void close();

    u16 localPort;
    u8  remoteIP[4];/*���÷�������IP��ַ*/
    u16 remotePort;/*����������һ���˿ڲ���ʼ��*/
private:
    SOCKET s;



};
#endif
