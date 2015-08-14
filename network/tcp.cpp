#include "tcp.h"
#include "ebox.h"
extern USART uart1;

int TCPCLIENT::begin(SOCKET ps,uint16 port)
{
	int ret = 0;
	s = ps;
	localPort = port;
	
	return ret;
}
int TCPCLIENT::connect(u8* IP,uint16 Port)
{
	int ret = 0;
	u8 i = 20;
	u8 tmp;
	remoteIP[0] = IP[0];
	remoteIP[1] = IP[1];
	remoteIP[2] = IP[2];
	remoteIP[3] = IP[3];
	remotePort = Port;
	uart1.printf("\r\nremote server:%d.%d.%d.%d:%d",remoteIP[0],remoteIP[1],remoteIP[2],remoteIP[3],remotePort);
	while(--i)
	{
	 tmp =eth->getSn_SR(s);
	 switch(tmp)/*获取socket0的状态*/
    {
		 case SOCK_INIT:
			 ret = _connect(s, remoteIP ,remotePort);/*在TCP模式下向服务器发送连接请求*/ 
		 	 uart1.printf("\r\nconnnect() return = %d",ret);
		 	 uart1.printf("\r\nconnecting to server...");
		 break;
		 case SOCK_ESTABLISHED:
			   if(eth->getSn_IR(s) & Sn_IR_CON)
         {
            eth->setSn_IR(s, Sn_IR_CON);/*Sn_IR的第0位置1*/
         }
				 uart1.printf("\r\nconnected !");
		 return ret;
		 case SOCK_CLOSED:
			ret = _socket(s,Sn_MR_TCP,localPort,Sn_MR_ND);/*打开socket的一个端口*/
		  uart1.printf("\r\nopen local port:%d,use %d socket",localPort,s);
		 	uart1.printf("\r\n_socket() return = %d",ret);
		 break;
		}
		delay_ms(500);
	}
	if(i == 0)
		ret = -1;
	return ret;
}
u16 TCPCLIENT::recv(u8* buf)
{
	u16 len = 0;

	if(eth->getSn_IR(s) & Sn_IR_RECV)
	{
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第0位置1*/
		len=eth->getSn_RX_RSR(s);/*len为已接收数据的大小*/
		if(len>0)
		{
			_recv(s,buf,len);/*W5500接收来自Sever的数据*/
				recvFlag = 0;
		}	
	}

		return len;
}
//u16 TCPCLIENT::interruptRecv(u8* buf)
//{
//	u16 len;
//	u8 tmp2,tmp3;

//	tmp2 = eth->getSIR();
//	tmp3 = eth->getSn_IR(s);
//	#if 0
//	uart1.printf("SIr:0x%02x",tmp2);
//	uart1.printf("Sn_Ir:0x%02x",tmp3);
//	#endif
//	if(tmp2&(1<<s))
//	{
//		eth->setSIR(1<<s);/*SIR的第0位置1*/
//	}
//	
//	if(tmp3&Sn_IR_RECV)
//	{
//		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR的第2位置1*/
//		
//		len=eth->getSn_RX_RSR(s);/*len为已接收数据的大小*/
//		if(len>0)
//		{
//			_recv(s,buf,len);/*W5500接收来自Sever的数据*/
//			recvFlag = 0;
//		}
//	}
//	
//	return len;
//}
u16 TCPCLIENT::send(u8* buf,u16 len)
{
	return _send(s,buf,len);
}
