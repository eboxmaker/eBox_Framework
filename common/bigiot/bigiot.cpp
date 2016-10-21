#include "bigiot.h"

#if 1
#define BIG_DEBUG(...) uart1.printf("\n[BIGIOT]:");uart1.printf(__VA_ARGS__)
#define BIG_INFO(...) uart1.printf(__VA_ARGS__)
#else
#define BIG_DEBUG(...)
#define BIG_INFO(...) 
#endif

bool BigIot::connect(String *remote_ip, uint32_t remote_port, uint16_t local_port)
{
    uint32_t last_time = millis();
    uint8_t recv_buf[30]={0};
    uint8_t len = 0;
    bool ret;
    
    if(connected()){
        BIG_DEBUG("connect success!");
        return true;
    }
    else{
        BIG_DEBUG("need reconnect...");
    }
    BigIotPort::connect(remote_ip,remote_port,local_port);
    BIG_DEBUG("connecting...");
    while(available() == 0){
        if(millis() - last_time > 6000){
            online = false;
            BIG_DEBUG("connect failed!");
           return false;
        }
        delay_ms(1000);
        BIG_INFO(".");
    };
    len = read_until((uint8_t *)recv_buf,'\n');

    cJSON * pJson =cJSON_Parse((const char*)recv_buf);
    cJSON * method = cJSON_GetObjectItem(pJson, "M");
    String M = method->valuestring;
    if(M == "WELCOME TO BIGIOT"){
        BIG_DEBUG("connect success!");
        ret =  true;
    }else{
        ret =  false;
    }
    cJSON_Delete(pJson);
    cJSON_Delete(method);
    
    return ret;

}    

bool BigIot::login(String *device_id,String *apikey)
{

    bool ret;
    uint16_t len = 0;
    uint8_t recv_buf[256]={0};
    uint32_t last_time = millis();
    String msg="{\"M\":\"checkin\",\"ID\":\"" + *device_id + "\",\"K\":\"" + *apikey + "\"}\n";
    
    ret = send((uint8_t*)msg.c_str(),msg.length());
    if(ret)
        BIG_DEBUG("login");
    while(available() == 0){
        if(millis() - last_time > 6000){
            online = false;
            BIG_DEBUG("login failed!please wait for 60s then try again!");
           return false;
        }
        delay_ms(1000);
        BIG_INFO(".");
    };
    len = read_until((uint8_t *)recv_buf,'\n');

    cJSON * pJson =cJSON_Parse((const char*)recv_buf);
    cJSON * method = cJSON_GetObjectItem(pJson, "M");
    String M = method->valuestring;
    if(M == "checkinok"){
        BIG_DEBUG("login success!");
        online = true;
    }else{
        BIG_DEBUG("login failed!");
        online = false;
    }
    cJSON_Delete(pJson);
    cJSON_Delete(method);
   
    return online;

}
bool BigIot::realtime_data(String *device_id,String *data_id,uint32_t val)
{
    uint16_t len;
    String VAL(val);
    String str = "{\"M\":\"update\",\"ID\":" + *device_id + ",\"V\":{\"" + *data_id + "\":" + VAL + "}}\n";
    BIG_DEBUG("send:");
    uart1.printf_length((const char *)str.c_str(), str.length());
    len = send((uint8_t *)str.c_str(), str.length());
    if(len == str.length()){
        return true;
    }
    else{
        return false;
    }
}


void BigIot::process_message(uint8_t *buf)
{
    uint16_t len=0;
    
    len = read_until(buf,'\n');
     String str((char *)buf);
   if(len > 0){
//        BIG_INFO("recv:%s",buf);
        if(str.startsWith("{") && str.endsWith("\n")){
            cJSON * pJson =cJSON_Parse((const char*)buf);
            cJSON * method = cJSON_GetObjectItem(pJson, "M");
            cJSON * content = cJSON_GetObjectItem(pJson, "C");
            cJSON * client_id = cJSON_GetObjectItem(pJson, "ID");
            String M = method->valuestring;
            if(M == "say"){
                String C = content->valuestring;
                String F_C_ID = client_id->valuestring;
                if(C == "play"){
                    PB8.reset();
                }else if(C == "stop"){
                    PB8.set();
                }
            }
            cJSON_Delete(pJson);
            cJSON_Delete(method);
            cJSON_Delete(content);
            cJSON_Delete(client_id);
        }
    }
    
}
