#include "udp.h"

u8 UDPSERVER::begin(SOCKET ps,uint16 port)
{
	u8 ret;
	localPort = port;
	s = ps;
	 ret = socket(s,Sn_MR_UDP,localPort,0);/*初始化socket 0的套接字*/
	return ret;
}
u8 UDPSERVER::closeSocket()
{
	close(s);
	return 0;
}
u8 UDPSERVER::process()
{
	u8 ret = 0;
  u16 len = 0;
	
	if(eth->getSn_IR(s) & Sn_IR_RECV)
	{
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第0位置1*/
		if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			len=recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500接收计算机发送来的数据*/
			if(len != sendto(s, buf,len, remoteIP, remotePort)) 
			{ 
			} 
		}
		ret = 1;
	}
	if(eth->getSn_IR(s) & Sn_IR_TIMEOUT)
	{
	}
	if(eth->getSn_IR(s) & Sn_IR_SEND_OK)
	{
	}
	return ret;
}

u8 UDPCLIENT::begin(SOCKET ps,uint16 port)
{
	u8 ret;
	s= ps;
	ret = socket(s,Sn_MR_UDP,port,0);/*初始化socket 0的套接字*/
	return ret;

}
u8 UDPCLIENT::send(u8* remoteIP,u16 remotePort,u8* buf,u16 len)
{
	len = sendto(s, buf,len, remoteIP, remotePort);
	return len;
}
u8 UDPCLIENT::process()
{
	u8 ret;
	if(eth->getSn_IR(s))
	{
		eth->setSn_IR(s, Sn_IR_SEND_OK);/*Sn_IR的第0位置1*/
	}
	return ret;
}
u8 UDPCLIENT::closeSocket()
{
	close(s);
	return 0;
}

