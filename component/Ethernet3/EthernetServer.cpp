#include "utility/w5500.h"
#include "utility/socket.h"
extern "C" {
#include "string.h"
}

#include "Ethernet3.h"
#include "EthernetClient.h"
#include "EthernetServer.h"

EthernetServer::EthernetServer(uint16_t port)
{
    _port = port;
}

void EthernetServer::begin()
{
//    ebox_printf("server begin\n");
    for (int sock = 0; sock < MAX_SOCK_NUM; sock++)
    {
        EthernetClient client(sock);
        if (client.status() == SnSR::CLOSED)
        {
            socket(sock, SnMR::TCP, _port, 0);
            listen(sock);
            EthernetClass::_server_port[sock] = _port;
//            ebox_printf("sock:%d status 0x%x\n",sock,client.status());
            break;
        }
//        ebox_printf("sock:%d status 0x%x\n",sock,client.status());
    }
}

void EthernetServer::accept()
{
    int listening = 0;
    for (int sock = 0; sock < MAX_SOCK_NUM; sock++)
    {
        EthernetClient client(sock);

        if (EthernetClass::_server_port[sock] == _port)
        {
            if (client.status() == SnSR::LISTEN)
            {
                listening = 1;
//                ebox_printf("listen sock:%d\n",sock);
            }
            else if (client.status() == SnSR::CLOSE_WAIT && !client.available())
            {
                client.stop();
            }
        }
//        ebox_printf("sock:%d port:%d\n",sock,EthernetClass::_server_port[sock]);

    }

    if (!listening)
    {
//        ebox_printf("重新初始化网络\n");
        begin();
    }
}
//uint32_t last_time = 0;
EthernetClient EthernetServer::available()
{
//        uint8_t buf[10];

//    uint32_t diff = millis() - last_time;
//    last_time = millis();
    accept();

    for (int sock = 0; sock < MAX_SOCK_NUM; sock++)
    {
//        ebox_printf("%d\n",diff);
        EthernetClient client(sock);
        if (EthernetClass::_server_port[sock] == _port &&
                (client.status() == SnSR::ESTABLISHED ||
                 client.status() == SnSR::CLOSE_WAIT))
        {
//            client.remoteIP(buf);
//            ebox_printf("remote ip:%d:%d:%d:%d   0x%x\n",buf[0],buf[1],buf[2],buf[3],client.status());
            if (client.available())
            {
                // XXX: don't always pick the lowest numbered socket.
                return client;
            }
        }
    }

    return EthernetClient(MAX_SOCK_NUM);
}

size_t EthernetServer::write(uint8_t b)
{
    return write(&b, 1);
}

size_t EthernetServer::write(const uint8_t *buffer, size_t size)
{
    size_t n = 0;

//    accept();

    for (int sock = 0; sock < MAX_SOCK_NUM; sock++)
    {
        EthernetClient client(sock);

        if (EthernetClass::_server_port[sock] == _port &&
                client.status() == SnSR::ESTABLISHED)
        {
            n += client.write(buffer, size);
        }
    }

    return n;
}
