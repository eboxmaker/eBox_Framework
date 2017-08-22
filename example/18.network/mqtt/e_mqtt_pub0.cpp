/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/


#include "string.h"
#include "ebox.h"
#include "w5500.h"

#include "MQTTPacket.h"
#include "transport.h"
#include "stdlib.h"
#include "mqtt_api.h"

int toStop = 0;

MQTT mqtt;


u8 mac[6] = {0x00, 0x08, 0xdc, 0x11, 0x11, 0x11}; /*定义Mac变量*/
u8 ip[4] = {192, 168, 1, 191}; /*定义lp变量*/
u8 sub[4] = {255, 255, 255, 0}; /*定义subnet变量*/
u8 gw[4] = {192, 168, 1, 1}; /*定义gateway变量*/
u8 dns[4] = {192, 168, 1, 1};


char host[] = "m2m.eclipse.org";
//char host[]="messagesight.demos.ibm.com";
u16 host_port = 1883;
W5500 w5500(&PC13, &PC14, &PC15, &spi2);

int ret;
u8 buf[6];

void callback(char *topic, int t_length, char *payload,  int length)
{
    uart1.printf("topick  :%.*s\r\n", t_length, topic);
    uart1.printf("payload: %.*s\r\n", length, payload);
    uart1.printf("-----------------\r\n", length, payload);
}
void setup()
{
    ebox_init();
    uart1.begin(9600);
    uart1.printf("uart is ok !\r\n");

    w5500.begin(2, mac, ip, sub, gw, dns);
    attach_eth_to_socket(&w5500);

    w5500.getMAC (buf);
    uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", buf[0], buf[1], buf[2], buf[3], buf[4], buf[5]);
    w5500.getIP (buf);
    uart1.printf("IP : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getSubnet(buf);
    uart1.printf("mask : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    w5500.getGateway(buf);
    uart1.printf("GW : %d.%d.%d.%d\r\n", buf[0], buf[1], buf[2], buf[3]);
    uart1.printf("Network is ready.\r\n");

    PB8.mode(OUTPUT_PP);

    if(mqtt.begin(1, 65533) == 1)
    {
        if(mqtt.set_server_domain((char *)host, 1883) == 1)
        {
            mqtt.connect();
        }
    }

}
int value = 0;
char string[10] = {0};
uint32_t tmp_time;
int main(void)
{
    setup();
    while(1)
    {
        if(mqtt.connected())
        {
            mqtt.loop();
            if(millis() - tmp_time > 500)
            {
                value++;
                sprintf(string, "value = %d", value);
                value %= 1000000;
                mqtt.publish((char *)"planets/earth", string);
            }
        }
        else
        {
            if(mqtt.begin(1, 65533) == 1)
            {
                if(mqtt.set_server_domain((char *)host, 1883) == 1)
                {
                    mqtt.connect();
                }
            }
        }
        if(millis() - tmp_time > 500)
        {
            tmp_time = millis();
            PB8.toggle();
        }

    }
}

