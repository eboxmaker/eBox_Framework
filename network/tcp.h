#ifndef __TCP_H
#define __TCP_H
#include "socket.h"

class TCPCLIENT
{
	public:
		u8 begin(SOCKET ps,uint16 port);
		u8 connect(u8* IP,uint16 Port);
		u16 process();
		u16 interruptRecv();
	  u16 send(u8* buf,u16 len);

	  u8 recvFlag;
		u16 localPort;
	  u8 remoteIP[4];/*���÷�������IP��ַ*/
		u16 remotePort;/*����������һ���˿ڲ���ʼ��*/
		u8 buf[2048];

	private:
		SOCKET s;
		

};
#endif
