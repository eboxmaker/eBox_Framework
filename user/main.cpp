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


BigIot bigiot(&wifi);

#define HOST    "www.bigiot.net"
uint16_t    remote_port = 8181;
uint16_t    local_port = 4321;
//用户、设备接口
//用户、设备接口
#define  USERID     "897"
#define  DEVICEID   "904"
#define  APIKEY     "c6c5ef884"
//实时数据接口
#define  DATA_ID        "839"
#define  TEMPERATURE_ID "847"
#define  HUMIDITY_ID    "848"

///////////////////////////////////
#define postingInterval 10000
uint8_t     recv_buf[1024] = {0};
uint16_t    len = 0;
uint32_t    count = 0;
void setup()
{
    String teststr = "{111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111111116789012345}";
    ebox_init();
    uart1.begin(115200);
    uart1.printf("-------------------------------\r\n");
    uart1.printf("\ntest :%s",teststr.c_str());
    uart1.printf("\ntest :len:%d",teststr.length());
    uart1.printf("\ntest :end with }?:%d\n",teststr.endsWith("}"));


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
    uint32_t lastCheckInTime = 0;
    setup();


    while(1)
    {
        if(!bigiot.connected()){
            if(!bigiot.connect(HOST, remote_port, local_port)){
                uart1.printf("\nTCP connect failed!");
                return 0;
            }else{
                uart1.printf("\nTCP connecte success!");
            }

                
        }
        
        if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
            lastCheckInTime = millis();
            bigiot.login(DEVICEID,APIKEY);            
        }   
        if(bigiot.available())
            bigiot.process_message(recv_buf);
        
        if(bigiot.connected()){

            if(millis() - last_rt_time > 4000 || last_rt_time == 0){
                last_rt_time = millis();
                const char msg[] = "123";                
                ret = bigiot.say(BIGIOT_USER,USERID,msg);
                ret = bigiot.realtime_data(DEVICEID,TEMPERATURE_ID, random(100));
                ret = bigiot.realtime_data(DEVICEID,HUMIDITY_ID, random(100));
            }
            if(millis() - last_get_time > 11000 || last_get_time == 0){
                last_get_time = millis();
                char msg[10];                
                ret =  bigiot.get_server_time(msg); 
                        bigiot.quarry_status();
            }        
        
        }
//        if(millis() - last_time > 1000)
//        {
//            last_time = millis();
//            uart1.printf("\nfree:%d",ebox_get_free());
//        
//        }


        
    }

}





