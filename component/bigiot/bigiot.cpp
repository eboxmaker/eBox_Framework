/**
  ******************************************************************************
  * @file    bigiot.cpp
  * @author  shentq
  * @version V0.1
  * @date    2016/10/26
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "bigiot.h"

#if 1
#define BIG_DEBUG(...) uart1.printf("\n[BIGIOT]:");uart1.printf(__VA_ARGS__)
#define BIG_SEND(...) uart1.printf("\n[BIG SEND]:");uart1.printf(__VA_ARGS__)
#define BIG_RECV(...) uart1.printf("\n[BIG RECV]:");uart1.printf(__VA_ARGS__)
#else
#define BIG_DEBUG(...)
#define BIG_RECV(...) 
#endif

bool BigIot::send_connect(const char *remote_ip, uint32_t remote_port, uint16_t local_port)
{
    bool ret;
    ret = BigIotPort::connect(remote_ip,remote_port,local_port);
    if(ret){
        BIG_DEBUG("TCP connecting...");
    }else{
        BIG_DEBUG("TCP connect failed!");
    }
    return ret;
}    

bool BigIot::send_login(const char *device_id,const char *apikey)
{
    String str_device_id(device_id);
    String str_apikey(apikey);

    bool ret;
    String msg="{\"M\":\"checkin\",\"ID\":\"" + str_device_id + "\",\"K\":\"" + str_apikey + "\"}\n";
    ret = send((uint8_t*)msg.c_str(),msg.length());
    if(ret){
        BIG_SEND("[%s]",msg.c_str());
        BIG_DEBUG("LOGIN......");
    }else{
        BIG_DEBUG("login cmd send error!");
    }
    return ret;

}
bool BigIot::send_logout(const char *device_id,const char *apikey)
{

    String str_device_id(device_id);
    String str_apikey(apikey);
    bool ret;
    
    String msg="{\"M\":\"checkout\",\"ID\":\""+str_device_id+"\",\"K\":\""+str_apikey+"\"}\n";
    ret = send((uint8_t*)msg.c_str(),msg.length());
    if(ret){
        BIG_SEND("[%s]",msg.c_str());
        BIG_DEBUG("logout......");
    }else{
        BIG_DEBUG("logout cmd send error!");
    }
    delay_ms(500);
    disconnect();
    delay_ms(500);
    while(connected());
    if( (!connected()))
    {
        online = false;
        BIG_DEBUG("logout success!");        
    }
    return ret;

}

bool BigIot::send_realtime_data(const char *device_id,char *val_list)
{
    String str_device_id(device_id);
    String str_buf(val_list);
    bool ret;
    String str = "{\"M\":\"update\",\"ID\":" + str_device_id + ",\"V\":" + str_buf +"}\n";
    ret = send((uint8_t *)str.c_str(), str.length());
    if(ret){
        BIG_SEND("[%s]",str.c_str());
    }else{
        BIG_DEBUG("realtime_data cmd send failed!");        
    }
    return ret;
}

bool BigIot::send_active_alert(const char *msg,BIGIOT_ALERT_TYPE type)
{
    String str_msg(msg);
    bool ret;
    String _type;
    switch((uint8_t)type)
    {
        case BIGIOT_EMAIL:
            _type = "email";
            break;
        case BIGIOT_WEIBO:
            _type = "weibo";
            break;
        case BIGIOT_QQ:
            _type = "qq";
            break;
        default:
            _type = "email";
            break;   
    }
    String str = "{\"M\":\"alert\",\"C\":\""+str_msg+"\",\"B\":\""+_type+"\"}\n";
    ret = send((uint8_t *)str.c_str(), str.length());
    if(ret){
        BIG_SEND("[%s]",str.c_str());
    }else{
        BIG_DEBUG("alert cmd send failed!");                
    }
    return ret;
}
bool BigIot::send_query_server_time()
{
    bool ret;
    String str = "{\"M\":\"time\",\"F\":\"Y-m-d H:i:s\"}\n";
    ret = send((uint8_t *)str.c_str(), str.length());
    if(ret){
        BIG_SEND("[%s]",str.c_str());
    }else{
        BIG_DEBUG("query server time cmd send failed!");                
    }
    return ret;
}
bool BigIot::send_query_status()
{
    bool ret;
    String str = "{\"M\":\"status\"}\n";
    ret = send((uint8_t *)str.c_str(), str.length());
    if(ret){
        BIG_SEND("[%s]",str.c_str());
    }else{
        BIG_DEBUG("query status cmd send failed!");                        
    }
    return ret;
}
bool BigIot::send_query_is_online(const char *id_list)
{
    String str_id_list(id_list);
    
    bool ret;
    String str = "{\"M\":\"isOL\",\"ID\":[\""+ str_id_list +"\"]}\n";
    ret = send((uint8_t *)str.c_str(), str.length());
    if(ret){
        BIG_SEND("[%s]",str.c_str());
    }else{
        BIG_DEBUG("query is online cmd send failed!");                        
    }
    
    return ret;

}


bool BigIot::send_say(BIGIOT_USER_TYPE type,const char *id,const char *msg,const char *sign)
{
    String str_id(id);
    String str_msg(msg);
    String str_sign(sign);
    bool ret;
    String str;
    
    if(sign != NULL){
        String _sign(*sign);
        str = "{\"M\":\"say\",\"ID\":\""+ str_id +"\",\"C\":\""+ str_msg +"\",\"SIGN\":\""+ str_sign +"\"}\n";
    }else{
        switch((uint8_t)type)
        {
            case BIGIOT_ALL:
                str = "{\"M\":\"say\",\"ID\":\"ALL\",\"C\":\""+ str_msg +"\"}\n";break;
            case BIGIOT_USER:
                str = "{\"M\":\"say\",\"ID\":\"U"+ str_id +"\",\"C\":\""+ str_msg +"\"}\n";break;
            case BIGIOT_DEVICE:
                str = "{\"M\":\"say\",\"ID\":\"D"+ str_id +"\",\"C\":\""+ str_msg +"\"}\n";break;
            case BIGIOT_GUSET:
                str = "{\"M\":\"say\",\"ID\":\"G"+ str_id +"\",\"C\":\""+ str_msg +"\"}\n";break;
        }
    }
   ret = send((uint8_t *)str.c_str(), str.length());
    if(ret){
        BIG_SEND("[%s]",str.c_str());
    }else{
        BIG_DEBUG("say cmd send failed!");                                
    }
    return ret;
}
bool BigIot::is_online()
{
    if(online)return true;
    else return false;
}

void BigIot::process_message(uint8_t *buf)
{
    uint16_t len=0;
    
    if(millis() - last_login_time > 60000){
        BIG_DEBUG("offline!!!!");
        online = false;
    }
    
    len = read_until(buf,'\n');
    if(len > 0){
        buf[len -1 ] = '\0';
        BIG_RECV("[%s],buf len =%d",(const char *)buf,len);
        String str=(const char *)buf;
        if(str.startsWith("{") && str.endsWith("}")){
            cJSON * pJson =cJSON_Parse((const char*)buf);
            cJSON * method = cJSON_GetObjectItem(pJson, "M");
            String M = method->valuestring;
            if(M == "say"){
                cJSON * content = cJSON_GetObjectItem(pJson, "C");
                cJSON * client_id = cJSON_GetObjectItem(pJson, "ID");
                String C = content->valuestring;
                String F_C_ID = client_id->valuestring;
                if(C == "play"){
                    PB8.reset();
                }else if(C == "stop"){
                    PB8.set();
                }
                cJSON_Delete(content);
                cJSON_Delete(client_id);
            }
            if(M == "time")
            {
                cJSON * content = cJSON_GetObjectItem(pJson, "T");
                String T = content->valuestring;
                BIG_DEBUG(T.c_str());
            }
            if(M == "checkinok"){
                last_login_time = millis();
                BIG_DEBUG("device checkin ok!");
                online = true;
            }
            if(M == "checked")
            {
                last_login_time = millis();
                BIG_DEBUG("device checkin ok!");
                online = true;
            
            }else if(M == "connected")
            {
                BIG_DEBUG("device connected but checkin error!");
                online = false;
            }
            cJSON_Delete(pJson);
            cJSON_Delete(method);
            /*
            add more code...
            */
        }else{
            BIG_DEBUG("recv data error!!!");            
        }
    }    
}


