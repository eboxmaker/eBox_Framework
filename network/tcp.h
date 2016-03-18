#ifndef __TCP_H
#define __TCP_H
#include "socket.h"
#include "w5500.h"

class TCPCLIENT
{
public:
    int     begin(SOCKET ps, uint16_t port);
    bool	connect(u8 *IP, uint16_t Port);

    uint8_t     status();//网卡当前的状态
    uint16_t	available();//接收缓冲区是否可用
    bool	    connected();//是否存在链接
    void	    stop();

    u16     recv(u8 *buf);
    u16     recv(u8 *buf, uint16_t len);
    u16 	send(u8 *buf, u16 len);

    u16 localPort;
    u8  remoteIP[4];/*配置服务器的IP地址*/
    u16 remotePort;/*定义计算机的一个端口并初始化*/


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
    u8  remoteIP[4];/*配置服务器的IP地址*/
    u16 remotePort;/*定义计算机的一个端口并初始化*/
private:
    SOCKET s;



};
#endif
