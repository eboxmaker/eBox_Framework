#include "tcp.h"
#include "ebox.h"


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
	DBG("\r\nremote server:%d.%d.%d.%d:%d",remoteIP[0],remoteIP[1],remoteIP[2],remoteIP[3],remotePort);
	while(--i)
	{
	 tmp =eth->getSn_SR(s);
	 switch(tmp)/*��ȡsocket0��״̬*/
    {
		 case SOCK_INIT:
			 ret = _connect(s, remoteIP ,remotePort);/*��TCPģʽ���������������������*/ 
		 	 DBG("\r\nconnnect() return = %d",ret);
		 	 DBG("\r\nconnecting to server...");
		 break;
		 case SOCK_ESTABLISHED:
			   if(eth->getSn_IR(s) & Sn_IR_CON)
         {
            eth->setSn_IR(s, Sn_IR_CON);/*Sn_IR�ĵ�0λ��1*/
         }
				 DBG("\r\nconnected !");
		 return ret;
		 case SOCK_CLOSED:
			ret = _socket(s,Sn_MR_TCP,localPort,Sn_MR_ND);/*��socket��һ���˿�*/
		  DBG("\r\nopen local port:%d,use %d socket",localPort,s);
		 	DBG("\r\n_socket() return = %d",ret);
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
		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR�ĵ�0λ��1*/
		len=eth->getSn_RX_RSR(s);/*lenΪ�ѽ������ݵĴ�С*/
		if(len>0)
		{
			_recv(s,buf,len);/*W5500��������Sever������*/
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
//	DBG("SIr:0x%02x",tmp2);
//	DBG("Sn_Ir:0x%02x",tmp3);
//	#endif
//	if(tmp2&(1<<s))
//	{
//		eth->setSIR(1<<s);/*SIR�ĵ�0λ��1*/
//	}
//	
//	if(tmp3&Sn_IR_RECV)
//	{
//		eth->setSn_IR(s, Sn_IR_RECV);/*Sn_IR�ĵ�2λ��1*/
//		
//		len=eth->getSn_RX_RSR(s);/*lenΪ�ѽ������ݵĴ�С*/
//		if(len>0)
//		{
//			_recv(s,buf,len);/*W5500��������Sever������*/
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

int TCPSERVER::begin(SOCKET ps,uint16 port)
{
	int ret = 0;
	u8 tmp;
	u8 i = 20;
	
	s = ps;
	localPort = port;
  DBG("\r\ncreat TCP server !");

	while(--i)
	{
	 tmp =eth->getSn_SR(s);
	 switch(tmp)/*��ȡsocket0��״̬*/
   {
         case SOCK_INIT:/*socket��ʼ�����*/
           ret = _listen(s);/*��TCPģʽ���������������������*/
					 if(ret == 0)
					 {
						 DBG("\r\nlisten on port:%d,socket:%d",localPort,s);
						 DBG("\r\nwait one connection !");
						 return ret;
					 }
					 else
						 DBG("\r\nerr code:%d",ret);
						 
           break;
         case SOCK_CLOSED:/*socket�ر�*/
           ret = _socket(s,Sn_MR_TCP,localPort,Sn_MR_ND);/*��socket��һ���˿�*/
				   if(ret == 0)
						 DBG("\r\nopen port:%d success !",localPort);
           break;
				 default :
					 DBG("\r\nerr code:%d",tmp);
					 break;

	 }
 }
	return ret;
}
u16 TCPSERVER::recv(u8* buf)
{
	u16 len = 0;
	u8 tmp;
	
	 tmp = eth->getSn_SR(s);
	 switch(tmp)/*��ȡsocket0��״̬*/
	 {
      case SOCK_ESTABLISHED:/*socket���ӽ���*/
				 if(eth->getSn_IR(s) & Sn_IR_CON)
				 {
						eth->setSn_IR(s, Sn_IR_CON);/*Sn_IR�ĵ�0λ��1*/
					  eth->getSn_DIPR(s,remoteIP);
						remotePort = eth->getSn_DPORT(s);
						DBG("\r\none tcp client connected !");
						DBG("\r\nclient info:%d.%d.%d.%d:%d !",remoteIP[0],remoteIP[1],remoteIP[2],remoteIP[3],remotePort);
				 }

				len = eth->getSn_RX_RSR(s);/*lenΪ�ѽ������ݵĴ�С*/
				if(len>0)
				{
					len = _recv(s,buf,len);/*W5200��������Sever������*/
				}
				break;
			case SOCK_LISTEN:
				break;
			case SOCK_CLOSE_WAIT:
				DBG("\r\n��������ر�");
				close();
				break;
			case SOCK_CLOSED:
				//DBG("\r\ntmp = %x",tmp);
				DBG("\r\n�����Ѿ��ر�,��ʼ���´򿪷�������");
				begin(s,localPort);
				break;
			
			default:
			  break;

	 }
		return len;
}
void TCPSERVER::close()
{
	 _close(s);
}
u16 TCPSERVER::send(u8* buf,u16 len)
{
	return _send(s,buf,len);/*W5200��Server��������*/
}

