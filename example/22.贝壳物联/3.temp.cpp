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

String  bk_string;
String  HOST = "www.bigiot.net";
uint16_t    remote_port = 8181;
uint16_t    local_port = 4321;

//用户、设备接口
String  USERID          = "897"; 
String  DEVICEID        = "904"; 
String  APIKEY          = "c6c5ef884"; 
//实时数据接口
String  DATA_ID         = "839";
String  TEMPERATURE_ID  = "847";
String  HUMIDITY_ID     = "848";
///////////////////////////////////
#define postingInterval 10000
String  temp;
uint8_t     recv_buf[1024] = {0};
uint16_t    len = 0;
uint32_t    count = 0;
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
    uint32_t lastCheckInTime = 0;
    setup();


    while(1)
    {
        if(!bigiot.connected()){
            if(!bigiot.connect(&HOST, remote_port, local_port)){
                uart1.printf("\nTCP connect failed!");
                return 0;
            }else{
                uart1.printf("\nTCP connecte success!");
            }

                
        }
        
        if(millis() - lastCheckInTime > postingInterval || lastCheckInTime==0) {
            lastCheckInTime = millis();
            bigiot.login(&DEVICEID,&APIKEY);            
        }   
        if(bigiot.available())
            bigiot.process_message(recv_buf);
        
        if(bigiot.connected()){

            if(millis() - last_rt_time > 4000 || last_rt_time == 0){
                last_rt_time = millis();
                String msg(random(100));
                ret = bigiot.say(BIGIOT_USER,&USERID,&msg);
                ret = bigiot.realtime_data(&DEVICEID,&TEMPERATURE_ID, random(100));
            }
            if(millis() - last_get_time > 11000 || last_get_time == 0){
                last_get_time = millis();
                ret =  bigiot.get_server_time(&temp); 
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





