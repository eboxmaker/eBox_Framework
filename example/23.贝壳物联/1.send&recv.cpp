/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "ESP8266.h"
#include "esp8266_tcp.h"
#include "wstring.h"
#include "bigiot.h"
//WIFI_TCP tcp(&wifi);

char *out;

BigIot bigiot(&wifi);

#define     HOST            "www.bigiot.net"
#define     postingInterval 5000
uint16_t    remote_port     = 8181;
uint16_t    local_port      = 4321;
//用户、设备接口
#define  USERID     "897"
#define  DEVICEID   "931"
#define  APIKEY     "d28fa984b"
//实时数据接口
#define  DATA_ID        "865"
#define  TEMPERATURE_ID "866"
#define  HUMIDITY_ID    "867"

///////////////////////////////////
uint8_t     recv_buf[1024] = {0};
uint16_t    len = 0;
uint32_t    count = 0;
char *make_data();

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("-------------------------------\r\n");

    PB8.mode(OUTPUT_PP);
    wifi.begin(&PA4, &uart2, 115200);
    wifi.join_ap();
    bigiot.begin();

}
int main(void)
{
    int ret;

    uint32_t last_time= 0;
    uint32_t last_rt_time = 0;
    uint32_t last_get_time = 0;
    uint32_t last_login_time = 0;
    setup();


    while(1)
    {
        if(!bigiot.connected()){
            if(!bigiot.connect(HOST, remote_port, local_port)){
                uart1.printf("\nTCP connect failed!");
            }else{
                uart1.printf("\nTCP connecte success!");
            }
        }
        
        if(millis() - last_login_time > postingInterval || last_login_time==0) {
            last_login_time = millis();
            bigiot.send_login(DEVICEID,APIKEY);            
        }   
        if(bigiot.available())
            bigiot.process_message(recv_buf);
        
        if(bigiot.is_online()){

            if(millis() - last_rt_time > 4000 || last_rt_time == 0){
                last_rt_time = millis();
                ret = bigiot.send_say(BIGIOT_USER,USERID,"say something");
                out = make_data();
                ret = bigiot.send_realtime_data(DEVICEID,(char *)out);
                ebox_free(out);
            }
            if(millis() - last_get_time > 11000 || last_get_time == 0){
                last_get_time = millis();
                ret =  bigiot.send_query_server_time(); 
                ret =  bigiot.send_query_status();
            }        
        
        }
        if(millis() - last_time > 1000)
        {
            last_time = millis();
            uart1.printf("\nfree:%d",ebox_get_free());
        
        }


        
    }

}

char *make_data()
{
    cJSON * pJsonRoot = NULL;
    pJsonRoot = cJSON_CreateObject();
    if(NULL == pJsonRoot){
        return NULL;
    }
    cJSON_AddNumberToObject(pJsonRoot, "865", random(100));
    cJSON_AddNumberToObject(pJsonRoot, "866", random(100));
    cJSON_AddNumberToObject(pJsonRoot, "867", random(100));

    char *p;
    p = cJSON_PrintUnformatted(pJsonRoot);
    if(NULL == p){
        cJSON_Delete(pJsonRoot);
        return NULL;
    }
        cJSON_Delete(pJsonRoot);
    return p;
}




