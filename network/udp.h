#ifndef __UDP_H
#define __UDP_H
#include "socket.h"
class UDPSERVER
{
	
	public:
		u8 begin(SOCKET ps,uint16 port);
		u8 process();
		u8 closeSocket();
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
		u8 send(u8* remoteIP,u16 remotePort,u8* buf,u16 len);
		u8 process();
		u8 closeSocket();

	private:
		SOCKET s;/*���屾�ص�SOCKET����*/

};

#endif
