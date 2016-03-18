#include "tcp.h"
#include "ebox.h"

#define TCP_DEBUG 1
#if TCP_DEBUG
#define TCP_DBG(...) DBG(__VA_ARGS__)
#else
#define  TCP_DBG(...)
#endif

int TCPCLIENT::begin(SOCKET ps, uint16_t port)
{
    s = ps;
    localPort = port;
    return 1;
}
//����Զ�̷�����
//����1�ɹ���0ʧ��
bool TCPCLIENT::connect(u8 *IP, uint16_t Port)
{
    int ret;
    u8 i = 20;
    u8 state;
    remoteIP[0] = IP[0];
    remoteIP[1] = IP[1];
    remoteIP[2] = IP[2];
    remoteIP[3] = IP[3];
    remotePort = Port;
    TCP_DBG("remote server:%d.%d.%d.%d:%d\r\n", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3], remotePort);
    while(--i)
    {
        state = status();
        switch(state)/*��ȡsocket0��״̬*/
        {
        case SOCK_INIT:
            TCP_DBG("sending connect cmd...\r\n", localPort, s);
            ret = _connect(s, remoteIP , remotePort); /*��TCPģʽ���������������������*/
            if(ret == 1)
            {
                TCP_DBG("send connect cmd ok !\r\n");
            }
            else
            {
                TCP_DBG("send connect cmd failed !\r\n");
            }
            break;
        case SOCK_ESTABLISHED:
            TCP_DBG("connecte successe!\r\n\r\n");
            return true;
        case SOCK_CLOSED:
            TCP_DBG("openning local socket...\r\n", localPort, s);
            ret = _socket(s, Sn_MR_TCP, localPort, Sn_MR_ND); /*��socket��һ���˿�*/
            if(ret == 1)
            {
                TCP_DBG("open local port:%d,use %d socket is ok!\r\n", localPort, s);
            }
            else
            {
                TCP_DBG("open local eth err : %d", ret);
            }
            break;
        }
        delay_ms(500);
    }
    if(i == 0)
        return false;

    return false;
}
//����socket״̬
uint8_t TCPCLIENT::status()
{
    return socket_status(s);
}

//���ؽ��ջ��������ȣ�0Ϊ��
uint16_t TCPCLIENT::available()
{
    return recv_available(s);
}

//�ж������Ƿ����
//1:����
//0:�Ͽ�
bool TCPCLIENT::connected()
{
    uint8_t tmp = status();
    return !(tmp == SOCK_CLOSED || tmp == SOCK_LISTEN || tmp == SOCK_FIN_WAIT || \
             (tmp == SOCK_CLOSE_WAIT && !available()));
}

//���ձ�׼�ĶϿ����ӹ��̣��Ͽ�����
void TCPCLIENT::stop()
{
    uint32_t start = millis();
    _disconnect(s);
    // wait a second for the connection to close
    while (status() != SOCK_CLOSED && millis() - start < 2000);
    if(status() == SOCK_CLOSED)
        _close(s);
}

//���ջ�������������
//������
//���ؽ��յ����ݵĳ��ȣ�0Ϊ��
u16 TCPCLIENT::recv(u8 *buf)
{
    u16 len = 0;
    if(connected())
    {
        len = available();/*lenΪ�ѽ������ݵĴ�С*/
        if(len > 0)
        {
            _recv(s, buf, len); /*W5500��������Sever������*/
        }
    }
    return len;
}
//��ȡ�ض�����
//������
//����ֵ �����յ��ĳ��ȣ�0Ϊû������
u16 TCPCLIENT::recv(u8 *buf, uint16_t len)
{
    uint16_t ret = 0;
    uint16_t llen;
    if(connected())
    {
        llen = available();/*llenΪ�ѽ������ݵĴ�С*/
        if(llen > 0)
        {
            if(len >= llen)
            {
                ret = _recv(s, buf, llen); /*W5500��������Sever������*/
            }
            else
            {
                ret = _recv(s, buf, len); /*W5500��������Sever������*/
            }
            //            TCP_DBG("��Ҫ�����ݳ��ȣ�%d\r\n����������:%d\r\n",len,llen);
        }
    }
    else
    {
        return ret;
        //       TCP_DBG("���ӶϿ���������\r\n");

    }
    return ret;
}

//���ط������ݵĳ��ȣ�0Ϊ����ʧ��
u16 TCPCLIENT::send(u8 *buf, u16 len)
{
    return _send(s, buf, len);
}

int TCPSERVER::begin(SOCKET ps, uint16_t port)
{
    int ret = 0;
    u8 tmp;
    u8 i = 20;

    s = ps;
    localPort = port;
    DBG("\r\ncreat TCP server !");

    while(--i)
    {
        tmp = socket_status(s);/*��ȡsocket��״̬*/
        switch(tmp)
        {
        case SOCK_INIT:/*socket��ʼ�����*/
            ret = _listen(s);/*��TCPģʽ���������������������*/
            if(ret == 1)
            {
                TCP_DBG("\r\nlisten on port:%d,socket:%d", localPort, s);
                TCP_DBG("\r\nwait one connection !");
                return ret;
            }
            else
                DBG("\r\nerr code:%d", ret);
            break;
        case SOCK_CLOSED:/*socket�ر�*/
            ret = _socket(s, Sn_MR_TCP, localPort, Sn_MR_ND); /*��socket��һ���˿�*/
            if(ret == 0)
                TCP_DBG("\r\nopen port:%d success !", localPort);
            break;
        default :
            TCP_DBG("\r\nerr code:%d", tmp);
            break;
        }
    }
    return ret;
}
u16 TCPSERVER::recv(u8 *buf)
{
    u16 len = 0;

    switch(socket_status(s)) /*��ȡsocket0��״̬*/
    {
    case SOCK_ESTABLISHED:/*socket���ӽ���*/
        if(client_connecte_event(s))
        {
            get_remote_ip(s, remoteIP);
            remotePort = get_remote_port(s);
            TCP_DBG("\r\none tcp client connected !");
            TCP_DBG("\r\nclient info:%d.%d.%d.%d:%d !", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3], remotePort);
        }
        len = recv_available(s);/*lenΪ�ѽ������ݵĴ�С*/
        if(len > 0)
        {
            len = _recv(s, buf, len); /*W5200��������Sever������*/
        }
        break;
    case SOCK_LISTEN:
        break;
    case SOCK_CLOSE_WAIT:
        TCP_DBG("\r\n��������ر�");
        close();
        break;
    case SOCK_CLOSED:
        //DBG("\r\ntmp = %x",tmp);
        TCP_DBG("\r\n�����Ѿ��ر�,��ʼ���´򿪷�������");
        begin(s, localPort);
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
u16 TCPSERVER::send(u8 *buf, u16 len)
{
    return _send(s, buf, len); /*W5200��Server��������*/
}

