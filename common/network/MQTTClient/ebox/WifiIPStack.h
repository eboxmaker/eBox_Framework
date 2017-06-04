#ifndef ARDUINOWIFIIPSTACK_H
#define ARDUINOWIFIIPSTACK_H

#include "ebox.h"
#include "esp8266.h"
#include "esp8266_tcp.h"


class WifiIPStack
{
public:
    WifiIPStack(WIFI_TCP* ptcp)
    {
        tcp = ptcp;
        tcp->begin();
    }

    int        connect(char* hostname, int port)
    {
        return tcp->connect(hostname, port, 7200);
    }

    int        read(unsigned char* buffer, int len, int timeout = 1000)
    {
        //set timeout
        if( tcp->available())
        {
            for(int i = 0; i < len; ++i)
            {

                if( tcp->available() > 0 )
                {
                    buffer[i] = tcp->read_onebyte();
                    //blkcat add for debug
                    //uart1.printf("-------------------> read:%d   %#x\n", i+1, buffer[i]);
                    // TODO: optimize
                    delay_ms(1);                      // This certainly is not a good way, but it works well.
                }
                else
                    return i;
            }
            return len;
        }
        else
        {
            return 0;
        }
    }

    int        write(unsigned char* buffer, int len, int timeout = 1000)
    {
        //set timeout
        return tcp->send((uint8_t*)buffer, len);
    }

    int        disconnect()
    {
        return tcp->disconnect();
    }

private:
    WIFI_TCP *      tcp;

};

#endif

