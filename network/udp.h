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
    u8 remoteIP[4];/*���÷�������IP��ַ*/
    u16 remotePort;/*����������һ���˿ڲ���ʼ��*/
    u16 localPort;/*���屾�ص�һ���˿ڲ���ʼ��*/
    SOCKET s;
    u8 recvFlag;


};

#endif
