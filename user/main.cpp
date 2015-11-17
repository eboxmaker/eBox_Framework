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
//	char host[] ={192,84,45,43};

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
int test();
int mqtt_publish(char *pTopic,char *pMessage);
int main(void)
{
	setup();
	while(1)
	{	
        mqtt_publish("123","myself test");
            delay_ms(2000);
	}
}

int test()
{
MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	char buf[200];
	int buflen = sizeof(buf);
	int mysock = 7;
	MQTTString topicString = MQTTString_initializer;
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
	int len = 0;
//	char *host = "m2m.eclipse.org";
	char host[] = {192,84,45,43};
	int port = 1883;



	transport_open(host,port);

	uart1.printf("Sending to hostname %s port %d\r\n", host, port);

	data.clientID.cstring = "me";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "testuser";
	data.password.cstring = "testpassword";
	data.MQTTVersion = 4;

	len = MQTTSerialize_connect((unsigned char *)buf, buflen, &data);

	topicString.cstring = "123";
	len += MQTTSerialize_publish((unsigned char *)(buf + len), buflen - len, 0, 0, 0, 0, topicString, (unsigned char *)payload, payloadlen);

	len += MQTTSerialize_disconnect((unsigned char *)(buf + len), buflen - len);

    delay_ms(500);
	rc = transport_sendPacketBuffer(mysock, (unsigned char *)buf, len);
	if (rc == len)
		uart1.printf("Successfully published\r\n");
	else
		uart1.printf("Publish failed\r\n");

exit:
	transport_close(mysock);

	return 0;
}
/**
  * @brief  ???(???)??????
  * @param  pTopic ????
  * @param  pMessage ????
  * @retval ??0??????
  */
int mqtt_publish(char *pTopic,char *pMessage)
{
  int32_t len=0;
    int rc = 0;
    char host[] = {192,84,45,43};

  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  unsigned char buf[200];
  MQTTString topicString = MQTTString_initializer;
  int msglen = strlen(pMessage);
  int buflen = sizeof(buf);
 
  data.clientID.cstring = "me";
  data.keepAliveInterval = 5;
  data.cleansession = 1;
  len = MQTTSerialize_connect(buf, buflen, &data); /* 1 */
 
  if(transport_open(host,1883))
        uart1.printf("open is ok\r\n");
  topicString.cstring = pTopic;
  len += MQTTSerialize_publish(buf + len, buflen - len, 0, 0, 0, 0, topicString, (unsigned char*)pMessage, msglen); /* 2 */
 
  len += MQTTSerialize_disconnect(buf + len, buflen - len); /* 3 */
  delay_ms(500);
  rc = transport_sendPacketBuffer(7,buf,len);
  if(rc > 0)
        uart1.printf("trans is ok,len = %d\r\n",rc);
  else
         uart1.printf("trans is failed,len = %d\r\n",rc);
     
  transport_close(7);
        if (rc == len)
               uart1.printf("Successfully published\r\n");
        else
                uart1.printf("Publish failed\r\n");
  return 0;
}

