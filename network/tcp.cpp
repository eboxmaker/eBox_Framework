#include "tcp.h"
#include "ebox.h"
extern USART uart1;

u8 TCPCLIENT::begin(SOCKET ps,uint16 port)
{
	u8 ret = 0;
	s = ps;
	localPort = port;
	
	return ret;
}
u8 TCPCLIENT::connect(u8* IP,uint16 Port)
{
	u8 ret = 0;
	u8 i = 50;
	u8 tmp;
	remoteIP[0] = IP[0];
	remoteIP[1] = IP[1];
	remoteIP[2] = IP[2];
	remoteIP[3] = IP[3];
	remotePort = Port;
	while(i--)
	{
	 tmp =eth->getSn_SR(s);
	 switch(tmp)/*��ȡsocket0��״̬*/
    {
		 case SOCK_INIT:
			 ret = _connect(s, remoteIP ,remotePort);/*��TCPģʽ���������������������*/ 
		   uart1.printf("\r\nserver:%d.%d.%d.%d:%d",remoteIP[0],remoteIP[1],remoteIP[2],remoteIP[3],remotePort);
		 	 uart1.printf("\r\nconnnect() return = 0x%x",ret);
		 	 uart1.printf("\r\nconnecting to server...");
		 break;
		 case SOCK_ESTABLISHED:
			   if(eth->getSn_IR(s) & Sn_IR_CON)
         {
            eth->setSn_IR(s, Sn_IR_CON);/*Sn_IR�ĵ�0λ��1*/
         }
				 uart1.printf("\r\nconnected !");
		 return ret;
		 case SOCK_CLOSED:
			ret = _socket(s,Sn_MR_TCP,localPort,Sn_MR_ND);/*��socket��һ���˿�*/
		  uart1.printf("\r\nopen local port:%d,use %d socket",localPort,s);
		 	uart1.printf("\r\n_socket() return = %d",ret);
		 break;
		}
		delay_ms(1000);
	}
	return ret;
}
u16 TCPCLIENT::recv(u8* buf)
{
	u16 len = 0;

	if(eth->getSn_IR(s) & Sn_IR_RECV)
	{
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR�ĵ�0λ��1*/
		len=eth->getSn_RX_RSR(s);/*lenΪ�ѽ������ݵĴ�С*/
		if(len>0)
		{
			_recv(s,buf,len);/*W5500��������Sever������*/
				recvFlag = 1;
		}	
	}

		return len;
}
u16 TCPCLIENT::interruptRecv(u8* buf)
{
	u16 len;
	u8 tmp2,tmp3;

	tmp2 = eth->getSIR();
	tmp3 = eth->getSn_IR(s);
	#if 0
	uart1.printf("SIr:0x%02x",tmp2);
	uart1.printf("Sn_Ir:0x%02x",tmp3);
	#endif
	if(tmp2&(1<<s))
	{
		eth->setSIR(1<<s);/*SIR�ĵ�0λ��1*/
	}
	
	if(tmp3&Sn_IR_RECV)
	{
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR�ĵ�2λ��1*/
		
		len=eth->getSn_RX_RSR(s);/*lenΪ�ѽ������ݵĴ�С*/
		if(len>0)
		{
			_recv(s,buf,len);/*W5500��������Sever������*/
			recvFlag = 1;
		}
	}
	
	return len;
}
u16 TCPCLIENT::send(u8* buf,u16 len)
{
	return _send(s,buf,len);
}
