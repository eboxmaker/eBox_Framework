#ifndef __MQTT_API_H
#define __MQTT_API_H
#include "socket.h"

#include "MQTTPacket.h"
#include "transport.h"



class MQTT
{
    public:
        char *name;
        char *password;
    
     static MQTTPacket_connectData data;
     unsigned char buf[200];

    public:
        MQTT(void)
        {
        };
        int begin(SOCKET p_s,uint16_t p_port);
        void set_server_domain(char *domain,uint16_t port);
        void set_server_ip(char ip[],uint16_t port);
        void set_user(const char *p_name,const char *p_pwd);
        int  connect();
        int  disconnect();
    
    
        int publish(char *topick,char *message);
        int subscribe(char *topick);
    private:
        SOCKET   local_socket;
        uint16_t local_port;
        uint8_t  server_ip[4];
        uint16_t server_port;
};
#endif
