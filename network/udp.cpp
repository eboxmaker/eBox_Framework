#include "udp.h"

u8 UDPSERVER::begin(SOCKET ps,uint16 port)
{
	u8 ret;
	localPort = port;
	s = ps;
	 ret = _socket(s,Sn_MR_UDP,localPort,0);/*初始化socket 0的套接字*/
	return ret;
}
u8 UDPSERVER::close()
{
	_close(s);
	return 0;
}
u8 UDPSERVER::recv()
{

  u16 len = 0;
	
	if(eth->getSn_IR(s) & Sn_IR_RECV)
	{
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第0位置1*/
		if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500接收计算机发送来的数据*/
			if(len != sendto(buf,len, remoteIP, remotePort)) 
			{ 
			} 
		}
	}
	return len;
}
u8 UDPSERVER::sendto(u8* remoteIP,u16 remotePort,u8* buf,u16 len)
{
	len = _sendto(s, buf,len, remoteIP, remotePort);
	return len;
}



//////////////////////////////////////////////////////////////////////////
u8 UDPCLIENT::begin(SOCKET ps,uint16 port)
{
	u8 ret;
	s= ps;
	localPort = port;
	ret = _socket(s,Sn_MR_UDP,localPort,0);/*初始化socket 0的套接字*/
	return ret;

}
u8 UDPCLIENT::sendto(u8* rIP,u16 rPort,u8* buf,u16 len)
{
	len = _sendto(s, buf,len, rIP, rPort);
	return len;
}
u8 UDPCLIENT::recv(u8* buf)
{
	u16 len = 0;
	if(eth->getSn_IR(s) & Sn_IR_SEND_OK)
	{
		eth->setSn_IR(s, Sn_IR_SEND_OK);/*Sn_IR的第0位置1*/
	}
	if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500接收计算机发送来的数据*/
			recvFlag = 1;
		}
	return len;
}
u8 UDPCLIENT::interruptRecv(u8* buf)
{
//	u8 ret;
	u16 len;
	u8 tmp2,tmp3;

		if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500接收计算机发送来的数据*/
		}

		return len;
}
u8 UDPCLIENT::close()
{
	_close(s);
	return 0;
}

