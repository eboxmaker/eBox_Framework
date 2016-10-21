#ifndef __BigIot_H
#define __BigIot_H
#include "bigiot_port.h"
#include "Wstring.h"
#include "cjson.h"


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
        bool connect(String *remote_ip, uint32_t remote_port, uint16_t local_port);        
        bool login(String *device_id,String *apikey);
        bool realtime_data(String *device_id,String *data_id,uint32_t val);
        void process_message(uint8_t *buf);
        
        
    bool   online;
private:
    String device_id;
    String apikey;


};
#endif
