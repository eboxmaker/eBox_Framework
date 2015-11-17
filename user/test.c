/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "string.h"
#include "ebox.h"
#include "w5500.h"
#include "tcp.h"
#include "dns.h"

extern void attach_eth_to_socket(W5500* e);
u8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};/*定义Mac变量*/
u8 ip[4]={192,168,1,199};/*定义lp变量*/
u8 sub[4]={255,255,255,0};/*定义subnet变量*/
u8 gw[4]={192,168,1,1};/*定义gateway变量*/
u8 dns[4] = {192,168,1,1};


DNS ddns;
	
W5500 w5500(&PC13,&PC14,&PC15,&spi2);
	
int ret;
u8 name[]="www.baidu.com";
u8 buf[6];

void setup()
{
	ebox_init();
	uart1.begin(9600);
    uart1.printf("uart is ok !\r\n");
	ret = sizeof(long);
	ret = sizeof(int);
	ret = sizeof(short int);
	ret = sizeof(short);

	w5500.begin(2,mac,ip,sub,gw,dns);

	
	attach_eth_to_socket(&w5500);
	
  w5500.getMAC (buf);
  uart1.printf("mac : %02x.%02x.%02x.%02x.%02x.%02x\r\n", buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
  w5500.getIP (buf);
  uart1.printf("IP : %d.%d.%d.%d\r\n", buf[0],buf[1],buf[2],buf[3]);
  w5500.getSubnet(buf);
  uart1.printf("mask : %d.%d.%d.%d\r\n", buf[0],buf[1],buf[2],buf[3]);
  w5500.getGateway(buf);
  uart1.printf("GW : %d.%d.%d.%d\r\n", buf[0],buf[1],buf[2],buf[3]);
  uart1.printf("Network is ready.\r\n");
	
	ddns.begin(SOCKET1,3000);
	
	ret = ddns.query(name);
	if(ret == DNS_RET_SUCCESS) /*发送DNS请求*/
		uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d] from %d.%d.%d.%d\r\n",name,ddns.domain_ip[0],ddns.domain_ip[1],ddns.domain_ip[2],ddns.domain_ip[3],dns[0],dns[1],dns[2],dns[3]);
	else if(ret == DNS_RET_FAIL)
    uart1.printf("获取超时\r\n");
	else
    uart1.printf("未知错误.\r\n");
}
#include "MQTTPacket.h"
#include "transport.h"
#include <signal.h>
#include "stdlib.h"
int toStop = 0;

void cfinish(int sig)
{
	signal(SIGINT, NULL);
	toStop = 1;
}

void stop_init(void)
{
	signal(SIGINT, cfinish);
	signal(SIGTERM, cfinish);
}
int main(void)
{
	setup();
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	int mysock = 7;
	unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
	int len = 0;
//	char *host = "m2m.eclipse.org";
	char host[] ={198,41,30,241};
    	int port = 1883;
stop_init();
//	if (argc > 1)
//		host = argv[1];

//	if (argc > 2)
//		port = atoi(argv[2]);

	uart1.printf("Sending to hostname %s port %d\n", host, port);
	mysock = transport_open(host, port);
	if(mysock < 0)
		return mysock;

	uart1.printf("Sending to hostname %s port %d\n", host, port);

	data.clientID.cstring = "me";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "testuser";
	data.password.cstring = "testpassword";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = transport_sendPacketBuffer(mysock, buf, len);

	/* wait for connack */
	if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;

		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			uart1.printf("Unable to connect, return code %d\n", connack_rc);
			goto exit;
		}
	}
	else
		goto exit;

	/* subscribe */
	topicString.cstring = "substopic";
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

	rc = transport_sendPacketBuffer(mysock, buf, len);
	if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) 	/* wait for suback */
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;

		rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			uart1.printf("granted qos != 0, %d\n", granted_qos);
			goto exit;
		}
	}
	else
		goto exit;

	/* loop getting msgs on subscribed topic */
	topicString.cstring = "pubtopic";
	while (!toStop)
	{
		/* transport_getdata() has a built-in 1 second timeout,
		your mileage will vary */
		if (MQTTPacket_read(buf, buflen, transport_getdata) == PUBLISH)
		{
			unsigned char dup;
			int qos;
			unsigned char retained;
			unsigned short msgid;
			int payloadlen_in;
			unsigned char* payload_in;
			int rc;
			MQTTString receivedTopic;

			rc = MQTTDeserialize_publish(&dup, &qos, &retained, &msgid, &receivedTopic,
					&payload_in, &payloadlen_in, buf, buflen);
			uart1.printf("message arrived %.*s\n", payloadlen_in, payload_in);
		}

		uart1.printf("publishing reading\n");
		len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
		rc = transport_sendPacketBuffer(mysock, buf, len);
	}

	uart1.printf("disconnecting\n");
	len = MQTTSerialize_disconnect(buf, buflen);
	rc = transport_sendPacketBuffer(mysock, buf, len);

exit:
    uart1.printf("end \r\n");
	transport_close(mysock);

	while(1)
	{	
        
	}
}




