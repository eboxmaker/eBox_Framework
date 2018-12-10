/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "bsp_ebox.h"

#include "../Ethernet3/utility/w5500.h"
#include "../Ethernet3/Ethernet3.h"
#include "../Ethernet3/EthernetUdp3.h"         // UDP library from: bjoern@cs.stanford.edu 12/30/2008
#include "../Ethernet3/dns.h"
#include <PubSubClient.h>

/**
	*	1	此例程需要调用eDrive目录下的w5500模块
	*	2	此例程演示了w5500的初始化，基本信息打印
	*/



/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"w5500 io test example"
#define EXAMPLE_DATE	"2018-08-11"


byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ip[] = {192, 168, 1, 177};  // <- change to match your network

//mosquitto.org
char server[] = "www.eboxmaker.com";


EthernetClient ethClient;
PubSubClient client(ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      client.subscribe("inTopic");
      client.publish("inTopic","hello world");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay_ms(5000);
    }
  }
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.print(" [");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.print("] ");
  Serial.println();
}


void setup()
{

    ebox_init();
    uart1.begin(115200);
  Ethernet.begin(mac);
    DNSClient dns;
    IPAddress remote_ip;
    dns.begin(Ethernet.dnsServerIP());
    int ret = dns.getHostByName(server,remote_ip);
    if (ret == 1)
    {
        uart1.print("server ip:[");
        remote_ip.printTo(uart1);
        uart1.println(']');
        client.setServer(server, 1883);
        client.setCallback(callback);
    }
    else
    {
        uart1.printf("connect failed!\r\n");
    }


  // Allow the hardware to sort itself out
  delay_ms(1500);
}

int main()
{
    setup();
    while(1)
    {
    
      if (!client.connected()) {
    reconnect();
  }
  client.loop();

    }

}