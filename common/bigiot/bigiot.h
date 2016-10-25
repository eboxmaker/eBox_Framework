#ifndef __BigIot_H
#define __BigIot_H
#include "bigiot_port.h"
#include "Wstring.h"
#include "cjson.h"
typedef enum{
    BIGIOT_ALL = 0 ,
    BIGIOT_USER = 1 ,
    BIGIOT_DEVICE = 2,
    BIGIOT_GUSET = 3
}BIGIOT_USER_TYPE;

typedef enum{
    BIGIOT_EMAIL = 0 ,
    BIGIOT_WEIBO = 1 ,
    BIGIOT_QQ = 2,
}BIGIOT_ALERT_TYPE;

class BigIot:public BigIotPort
{
public:
    BigIot(void *wifi):BigIotPort(wifi){
            online = false;
        };
        bool begin(){
            BigIotPort::begin(); 
            return EOK;
        };
        bool send_connect(const char *remote_ip, uint32_t remote_port, uint16_t local_port);        
        bool send_login(const char *device_id,const char *apikey);
        bool send_logout(const char *device_id,const char *apikey);
        bool send_realtime_data(const char *device_id,char *buf);
        bool send_say(BIGIOT_USER_TYPE type,const char *id,const char *msg,const char *sign = NULL);
        bool send_active_alert(const char *msg,BIGIOT_ALERT_TYPE type);
        bool send_query_server_time();
        bool send_query_status();
        bool send_query_is_online(const char *id_list);
        void process_message(uint8_t *buf);
        bool is_online();
        
private:
    bool   online;
    uint64_t last_login_time;


};
#endif
