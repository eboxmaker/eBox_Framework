#if !defined(ETHIPSTACK_H)
#define ETHIPSTACK_H

#include "ebox.h"
#include "tcp.h"
#include "dns.h"
#include "w5500.h"

class EthIPStack
{
public:
    EthIPStack()
    {
        has_ip = 0;
        tcp.begin(SOCKET7, 3000);
    }

    int        dnsquery(char* name)
    {
        int         ret;
        DNS         ddns;
        ddns.begin(SOCKET7, 2000);
        ret = ddns.query((char*)name);
        if(ret == 1)
            uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d]\n", name, ddns.domain_ip[0], ddns.domain_ip[1], ddns.domain_ip[2], ddns.domain_ip[3]);
        else if(ret == -1)
            uart1.printf("未知请求\r\n");
        else if(ret == -2)
            uart1.printf("获取超时\r\n");

        hostip[0] = ddns.domain_ip[0];
        hostip[1] = ddns.domain_ip[1];
        hostip[2] = ddns.domain_ip[2];
        hostip[3] = ddns.domain_ip[3];
        return 1;
    }

    int        connect(char* hostname, int port)
    {
        if( !has_ip )
            dnsquery(hostname);
        return tcp.connect(hostip, port);
    }

    int        connect(u8 hostname[], int port)
    {
        return tcp.connect(hostname, port);
    }

    int        read(unsigned char* buffer, int len, int timeout = 1000)
    {
        int         interval = 10; // all times are in milliseconds
        int         total = 0, rc = -1;

        if (timeout < 30)
            interval = 2;
        while (tcp.available() < len && total < timeout)
        {
            delay_ms(interval);
            total += interval;
        }
        if (tcp.available() >= len)
            rc = tcp.recv((uint8_t*)buffer, len);
        //uart1.printf("Calling read \n");
        return rc;
    }

    int        write(unsigned char* buffer, int len, int timeout = 1000)
    {
        //tcp.setTimeout(timeout);
        //uart1.printf("Calling write \n");
        return tcp.send((uint8_t*)buffer, len);
    }

    int        disconnect()
    {
        tcp.stop();
        return 0;
    }

private:
    TCPCLIENT           tcp;
    u8                  hostip[4];
    int                 has_ip;
};

#endif



