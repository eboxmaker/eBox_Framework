#ifndef __MQTT_API_H
#define __MQTT_API_H
#include "socket.h"

#include "MQTTPacket.h"
#include "transport.h"


#define MQTT_CALLBACK_SIGNATURE void (*callback)(char*,int, char*, int)
    
class MQTT
{
    public:
        char        *user_name;
        char        *password;
        int         anonymous;
        uint32_t    last_ping_time;
        uint8_t     buf[128];

        MQTTPacket_connectData data;

    public:

        int  begin(SOCKET p_s,uint16_t p_port);
        int  set_server_domain(char *domain,uint16_t port);
        void set_server_ip(char ip[],uint16_t port);
        void set_user(const char *p_name,const char *p_pwd);
        int  connect();
        int  connected();
        int  disconnect();
    
    
        int     publish(char *topick,char *message);
        int     subscribe(char *topick);
        void    loop();
        void   setCallback(MQTT_CALLBACK_SIGNATURE);
    private:
        SOCKET   local_socket;
        uint16_t local_port;
        uint8_t  server_ip[4];
        uint16_t server_port;
        MQTT_CALLBACK_SIGNATURE;


};
#endif
