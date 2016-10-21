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


BigIot BigIot(&wifi);

String  bk_string;
String  HOST = "www.bigiot.net";
String  DEVICEID="904"; 
String  APIKEY = "2571fd146"; 
String  DATA_ID = "839";

uint16_t    remote_port = 8181;
uint16_t    local_port = 4321;
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
    BigIot.begin();

}

//uint8_t bk_buf[]="{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
int main(void)
{
    int ret;
    uint32_t last_time=millis();
    uint32_t last_time1=millis();
    setup();

    while(1)
    {
        if(BigIot.online == true){

            BigIot.process_message(recv_buf);
            if(millis() - last_time > 4000){
                last_time = millis();
                ret = BigIot.realtime_data(&DEVICEID,&DATA_ID, random(100));
            }
        }
        else{
            ret = BigIot.connect(&HOST, remote_port, local_port);
            ret = BigIot.login(&DEVICEID,&APIKEY);           
            if(BigIot.online == false && BigIot.connected()){
                last_time=millis();
                while(millis() - last_time < 10000){
                    uart1.printf("%d\n",10 - (millis() - last_time)/1000);
                    delay_ms(1000);
                }
            }
        }
        if(millis() - last_time1 > 100)
        {
            last_time1 = millis();
            uart1.printf("free:%d\n",ebox_get_free());
        
        }

        
    }

}





