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
#include "dht11.h"
#include "oled_ssd1306.h"
#include "font.h"
//WIFI_TCP tcp(&wifi);

char *out;


BigIot bigiot(&wifi);
Dht11 sensor(&PB2);
OLED_SSD1306 oled(&PD3, &PD4, &PD5, &PD6, &PD7);

#define     HOST            "10.42.0.75"
#define     postingInterval 5000
uint16_t    remote_port     = 8181;
uint16_t    local_port      = 4321;
//???????
#define  USERID     "1207"
#define  DEVICEID   "1566"
#define  APIKEY     "4f394031d"
//??????
#define  DATA_ID        "865"
#define  TEMPERATURE_ID "1627"
#define  HUMIDITY_ID    "1628"

///////////////////////////////////
uint8_t     recv_buf[1024] = {0};
uint16_t    len = 0;
uint32_t    count = 0;

typedef  enum 
{
	Alert_Tem = 1,
	Alert_Hum = 2
}Alert_num;

char *make_data();
int is_alert();

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("-------------------------------\r\n");

    PB8.mode(OUTPUT_PP);
    wifi.begin(&PA4, &uart2, 115200);
    wifi.join_ap("Thindrome", "YgNel5mX");
    bigiot.begin();
	oled.begin();
}
int main(void)
{
    int ret;

    uint32_t last_time= 0;
    uint32_t last_rt_time = 0;
    uint32_t last_get_time = 0;
    uint32_t last_login_time = 0;
	uint32_t last_alert_time = 0;
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
							uart1.printf("1321%s", out);
                ebox_free(out);
            }
            if(millis() - last_get_time > 11000 || last_get_time == 0){
                last_get_time = millis();
                ret =  bigiot.send_query_server_time(); 
                ret =  bigiot.send_query_status();
            }     
			if(millis() - last_alert_time > 600000 || last_alert_time == 0) {
				if(is_alert()) {
					last_alert_time = millis();
				}
				
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
		sensor.read();
	uart1.printf("Temperature:%d  Humidity:%d\n", sensor.getTemperature(), sensor.getHumidity());
 //   cJSON_AddNumberToObject(pJsonRoot, "865", random(100));
    cJSON_AddNumberToObject(pJsonRoot, TEMPERATURE_ID, sensor.getTemperature());
    cJSON_AddNumberToObject(pJsonRoot, HUMIDITY_ID, sensor.getHumidity());

    char *p;
    p = cJSON_PrintUnformatted(pJsonRoot);
    if(NULL == p){
        cJSON_Delete(pJsonRoot);
        return NULL;
    }
        cJSON_Delete(pJsonRoot);
    return p;
}

int is_alert()
{	
	int flag1 = 0, flag2 = 0;
	if(sensor.getTemperature() > 30) {
		flag1++;
		
	}
	if(sensor.getHumidity() > 20) {
		flag2++;
	}
	if(flag1 == 1 && flag2 == 1) {
		bigiot.send_active_alert("temputer and humidity are too high", BIGIOT_QQ);
		return 1;
	}
	else if (flag1 == 1) {
		bigiot.send_active_alert("temputer is too high",BIGIOT_QQ);
		return 1;
	}
	else if (flag2 == 1) {
		bigiot.send_active_alert("humidity is too high",BIGIOT_QQ);
		return 1;
	}
	else
		return 0;
	
}



