#ifndef __BIGIOT_PORT_H
#define __BIGIOT_PORT_H
#include "esp8266_tcp.h"
#include "Wstring.h"


class BigIotPort: public WIFI_TCP
{
public:
        BigIotPort(void *wifi):WIFI_TCP((ESP8266*)wifi){ };
        bool begin(){
            WIFI_TCP::begin(); 
            return true;
        };
        bool connect(const char *remote_ip, uint32_t remote_port, uint16_t local_port);  
        bool connected();
        uint16_t available();
        uint16_t read_until(uint8_t *buf,char ch);
        uint16_t send(uint8_t *buf,uint16_t length);      
};


#endif
