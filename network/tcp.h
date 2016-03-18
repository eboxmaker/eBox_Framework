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
