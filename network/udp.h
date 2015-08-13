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
	  u8 remoteIP[4];/*���÷�������IP��ַ*/
		u16 remotePort;/*����������һ���˿ڲ���ʼ��*/
		u16 localPort;/*����������һ���˿ڲ���ʼ��*/
		SOCKET s;/*����������һ���˿ڲ���ʼ��*/
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
		u8 remoteIP[4];/*���÷�������IP��ַ*/
		u16 remotePort;/*����������һ���˿ڲ���ʼ��*/
		u16 localPort;/*���屾�ص�һ���˿ڲ���ʼ��*/
		SOCKET s;
		u8 recvFlag;

	private:

};

#endif
