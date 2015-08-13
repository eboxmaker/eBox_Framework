#ifndef __UDP_H
#define __UDP_H
#include "socket.h"
class UDPSERVER
{
	
	public:
		u8 begin(SOCKET ps,uint16 port);
		u8 sendto(u8* remoteIP,u16 remotePort,u8* buf,u16 len);
		u8 recv();
		u8 close();
	public:
	  u8 remoteIP[4];/*配置服务器的IP地址*/
		u16 remotePort;/*定义计算机的一个端口并初始化*/
		u16 localPort;/*定义计算机的一个端口并初始化*/
		SOCKET s;/*定义计算机的一个端口并初始化*/
		u8 buf[2048];
};
class UDPCLIENT
{
	public:
		u8 begin(SOCKET ps,uint16 port);
		u8 sendto(u8* remoteIP,u16 remotePort,u8* buf,u16 len);
		u8 recv(u8* buf);
		u8 interruptRecv(u8* buf);

		u8 close();
	
	public:
		u8 remoteIP[4];/*配置服务器的IP地址*/
		u16 remotePort;/*定义计算机的一个端口并初始化*/
		u16 localPort;/*定义本地的一个端口并初始化*/
		SOCKET s;
		u8 recvFlag;

	private:

};

#endif
