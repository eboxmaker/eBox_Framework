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
        bool connect(const char *remote_ip, uint32_t remote_port, uint16_t local_port);        
        bool login(const char *device_id,const char *apikey);
        bool logout(const char *device_id,const char *apikey);
        bool realtime_data(const char *device_id,const char *data_id,uint32_t val);
        bool get_server_time(const char *date_time);
        bool say(BIGIOT_USER_TYPE type,const char *id,const char *msg,const char *sign = NULL);
        bool active_alert(const char *msg,BIGIOT_ALERT_TYPE type);
        bool quarry_status();
        bool quarry_is_online(const char *id_list);
        void process_message(uint8_t *buf);
        
        
    bool   online;
private:
    String device_id;
    String apikey;


};
#endif
