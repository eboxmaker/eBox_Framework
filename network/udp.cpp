#include "udp.h"

u8 UDP::begin(SOCKET ps,uint16_t port)
{
	u8 ret;
	s= ps;
	localPort = port;
	ret = _socket(s,Sn_MR_UDP,localPort,0);/*初始化socket 0的套接字*/
	return ret;

}
u16 UDP::send(UDPMessage *msg)
{
	return _sendto(s, msg->buf,msg->len, msg->rIP, msg->rPort);
}
u16 UDP::sendto(u8* rIP,u16 rPort,u8* buf,u16 len)
{
	len = _sendto(s, buf,len, rIP, rPort);
	return len;
}
u16 UDP::recv(u8* buf)
{
	u16 len = 0;
//		eth->setSIR(1<<s);/*SIR的第s位置1*/
//		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第2位置1*/

	 if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第2位置1，清除中断标志位*/
			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500接收计算机发送来的数据*/
			recvFlag = 0;
		}
	return len;
}
u16 UDP::interrupt_recv(u8* buf)
{
//	u8 ret;
	u16 len = 0;

	if(eth->getSIR()&(1<<s))
	{
		eth->setSIR(1<<s);/*SIR的第0位置1*/
	}
	
	if(eth->getSn_IR(s)&Sn_IR_RECV)
	{
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第2位置1，清除中断标志位*/
		if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500接收计算机发送来的数据*/
		}
	}	

		return len;
}
u8 UDP::close()
{
	_close(s);
	return 0;
}

