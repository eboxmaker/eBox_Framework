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
String  APIKEY          = "2571fd146"; 
//实时数据接口
String  DATA_ID         = "839";
String  TEMPERATURE_ID  = "847";
String  HUMIDITY_ID     = "848";
///////////////////////////////////
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
    uint32_t last_time=millis();
    uint32_t last_time1=millis();
    uint32_t last_time2=millis();
    setup();

    while(1)
    {
        if(bigiot.online == true && bigiot.connected()){

            bigiot.process_message(recv_buf);
            if(millis() - last_time > 4000){
                last_time = millis();
                String user("897");
                String msg(random(100));
                ret = bigiot.say(BIGIOT_USER,&user,&msg);
                ret = bigiot.realtime_data(&DEVICEID,&TEMPERATURE_ID, random(100));
            }
            if(millis() - last_time2 > 11000){
                last_time2 = millis();
                ret =  bigiot.get_server_time(&temp); 
            }        
        }else{
            ret = bigiot.connect(&HOST, remote_port, local_port);
            ret = bigiot.login(&DEVICEID,&APIKEY);           
            if(bigiot.online == false && bigiot.connected()){
                last_time=millis();
                ret = bigiot.logout(&DEVICEID,&APIKEY);
                while(millis() - last_time < 10000){
                    uart1.printf("%d\n",10 - (millis() - last_time)/1000);
                    delay_ms(1000);
                }

            }
        }
        if(millis() - last_time1 > 1000)
        {
            last_time1 = millis();
            uart1.printf("free:%d\n",ebox_get_free());
        
        }


        
    }

}





