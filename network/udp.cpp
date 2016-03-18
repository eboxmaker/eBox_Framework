#include "udp.h"

u8 UDP::begin(SOCKET ps, uint16_t port)
{
    u8 ret;
    s = ps;
    localPort = port;
    ret = _socket(s, Sn_MR_UDP, localPort, 0); /*初始化socket 0的套接字*/
    return ret;

}
u16 UDP::send(UDPMessage *msg)
{
    return _sendto(s, msg->buf, msg->len, msg->rIP, msg->rPort);
}
u16 UDP::sendto(u8 *rIP, u16 rPort, u8 *buf, u16 len)
{
    len = _sendto(s, buf, len, rIP, rPort);
    return len;
}
u16 UDP::recv(u8 *buf)
{
    u16 len = 0;
    if((len = recv_available(s)) > 0)
    {
        len = _recvfrom(s, buf, len, remoteIP, &remotePort); /*W5500接收计算机发送来的数据*/
        recvFlag = 0;
    }
    return len;
}
u8 UDP::close()
{
    _close(s);
    return 0;
}

