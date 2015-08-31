#include "udp.h"

u8 UDP::begin(SOCKET ps,uint16 port)
{
	u8 ret;
	s= ps;
	localPort = port;
	ret = _socket(s,Sn_MR_UDP,localPort,0);/*��ʼ��socket 0���׽���*/
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
//		eth->setSIR(1<<s);/*SIR�ĵ�sλ��1*/
//		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR�ĵ�2λ��1*/

	 if((len=eth->getSn_RX_RSR(s))>0)
		{ 
			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500���ռ����������������*/
			recvFlag = 0;
		}
	return len;
}
//u8 UDP::interruptRecv(u8* buf)
//{
////	u8 ret;
//	u16 len = 0;
//	u8 tmp2,tmp3;

//	tmp2 = eth->getSIR();
//	tmp3 = eth->getSn_IR(s);
//	#if 0
//	uart1.printf("\r\nSIR:0x%02x",tmp2);
//	uart1.printf("\r\nSn_IR:0x%02x",tmp3);
//	#endif
//	if(tmp2&(1<<s))
//	{
//		eth->setSIR(1<<s);/*SIR�ĵ�0λ��1*/
//	}
//	
//	if(tmp3&Sn_IR_RECV)
//	{
//		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR�ĵ�2λ��1*/
//		if((len=eth->getSn_RX_RSR(s))>0)
//		{ 
//			len=_recvfrom(s, buf, len, remoteIP,&remotePort);/*W5500���ռ����������������*/
//		}
//		recvFlag = 0;
//	}	

//		return len;
//}
u8 UDP::close()
{
	_close(s);
	return 0;
}

