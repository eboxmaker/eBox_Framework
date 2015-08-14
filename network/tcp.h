#ifndef __TCP_H
#define __TCP_H
#include "socket.h"

class TCPCLIENT
{
	public:
		int begin(SOCKET ps,uint16 port);
		int connect(u8* IP,uint16 Port);
		u16 recv(u8* buf);
//		u16 interruptRecv(u8* buf);
	  u16 send(u8* buf,u16 len);

	  u8  recvFlag;
		u16 localPort;
	  u8  remoteIP[4];/*���÷�������IP��ַ*/
		u16 remotePort;/*����������һ���˿ڲ���ʼ��*/


	private:
		SOCKET s;

};

class TCPSERVER
{
	public:
		int begin(SOCKET ps,uint16 port);
		u16 recv(u8* buf);
	  u16 send(u8* buf,u16 len);
	  void close();
	
		u8  recvFlag;
		u16 localPort;
	  u8  remoteIP[4];/*���÷�������IP��ַ*/
		u16 remotePort;/*����������һ���˿ڲ���ʼ��*/
	private:
		SOCKET s;
  
	

};
#endif
