#include "string.h"
#include "mqtt_api.h"
#include "dns.h"

#include "MQTTPacket.h"
#include "transport.h"

#define MQTT_DEBUG 1
#if MQTT_DEBUG
	#define MQTT_DBG(...) DBG(__VA_ARGS__)
#else
	#define  MQTT_DBG(...) 
#endif

int MQTT::begin(SOCKET p_s,uint16_t p_port)
{
    local_socket = p_s;
    local_port = p_port;
    return 0;
}
void MQTT::set_server_domain(char *domain,uint16_t port)
{
    int ret;
    DNS dns;	
    dns.begin(1,port);
    
	MQTT_DBG("-----start----------\r\n");    
	ret = dns.query(domain);
	if(ret == 1) /*发送DNS请求*/
    {
        dns.get_domain_ip(server_ip);
		MQTT_DBG("IP     : [%d.%d.%d.%d]\r\n",dns.domain_ip[0],dns.domain_ip[1],dns.domain_ip[2],dns.domain_ip[3]);
    }
	else {
        MQTT_DBG("解析错误：ret= %d\r\n",ret);     
    }
	MQTT_DBG("------end---------\r\n\r\n",domain);    

    server_port = port;
}
void MQTT::set_server_ip(char ip[],uint16_t port)
{
    server_ip[0] = ip[0];
    server_ip[1] = ip[1];
    server_ip[2] = ip[2];
    server_ip[3] = ip[3];
    server_port = port;
    
}
void MQTT::set_user(const char *p_name,const char *p_pwd)
{
    name = (char *)p_name;
    password = (char *)p_pwd;
}

int MQTT::publish(char *topick,char *message)
{
  int32_t len=0;
   int rc = 0;
  MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
  unsigned char buf[200];
  MQTTString topicString = MQTTString_initializer;
  int msglen = strlen(message);
  int buflen = sizeof(buf);
    
  transport_init(local_socket,local_port);
 
  data.clientID.cstring = "me1";
  data.keepAliveInterval = 5;
  data.cleansession = 1;
  topicString.cstring = topick;
   //组织消息
  len = MQTTSerialize_connect(buf, buflen, &data); /* 1 */
  len += MQTTSerialize_publish(buf + len, buflen - len, 0, 0, 1, 0, topicString, (unsigned char*)message, msglen); /* 2 */
  len += MQTTSerialize_disconnect(buf + len, buflen - len); /* 3 */
  
    MQTT_DBG("----------start:publish-----------------\r\n");
 
    //链接服务器
    rc = transport_open((char *)server_ip,server_port);
    if(rc == 1)
        MQTT_DBG("connect mqtt server is ok !\r\n");
    else
        MQTT_DBG("connect mqtt server is failed:%d\r\n",rc);
  //发布消息
//    while(1)
//    {
      rc = transport_sendPacketBuffer(buf,len);
      if(rc > 0){
          MQTT_DBG("topick  : %s\r\n",topick);
          MQTT_DBG("message : %s\r\n",message);
          MQTT_DBG("trans is ok,len = %d\r\n",rc);
      }
      else
      {
            MQTT_DBG("trans is failed,len = %x\r\n",rc);
            MQTT_DBG("---------------------------\r\n");
          //break;
      }
//      delay_ms(1000);
//      
//     }
  transport_close(local_socket);
        if (rc == len)
               MQTT_DBG("Successfully published\r\n");
        else
                MQTT_DBG("Publish failed\r\n");
  MQTT_DBG("-----------end------------------\r\n\r\n");
  return 0;
}
int MQTT::subscribe(char *topick)
{

    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;
	int rc = 0;
	int mysock = 0;
	unsigned char buf[200];
	int buflen = sizeof(buf);
	int msgid = 1;
	MQTTString topicString = MQTTString_initializer;
	int req_qos = 0;
	char* payload = "mypayload";
	int payloadlen = strlen(payload);
	int len = 0;


  transport_init(local_socket,local_port);


   MQTT_DBG("----------start:subscribe-----------------\r\n");
    //链接服务器
    rc = transport_open((char *)server_ip,server_port);
  if(rc != 0)
        MQTT_DBG("open is ok status = %d\r\n",rc);
  else
      MQTT_DBG("open failed\r\n");

   data.clientID.cstring = "Client55249";
	data.keepAliveInterval = 20;
	data.cleansession = 1;
	data.username.cstring = "testuser";
	data.password.cstring = "testpassword";

	len = MQTTSerialize_connect(buf, buflen, &data);
	rc = transport_sendPacketBuffer( buf, len);

	/* wait for connack */
	if (MQTTPacket_read(buf, buflen, transport_getdata) == CONNACK)
	{
		unsigned char sessionPresent, connack_rc;

		if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0)
		{
			MQTT_DBG("Unable to connect, return code %d\n", connack_rc);
			goto exit;
		}
	}
	else
		goto exit;

	/* subscribe */
	topicString.cstring = topick;
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

	rc = transport_sendPacketBuffer( buf, len);
	if (MQTTPacket_read(buf, buflen, transport_getdata) == SUBACK) 	/* wait for suback */
	{
		unsigned short submsgid;
		int subcount;
		int granted_qos;

		rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
		if (granted_qos != 0)
		{
			MQTT_DBG("granted qos != 0, %d\n", granted_qos);
			goto exit;
		}
        MQTT_DBG("subscribe:ok!\r\n");
        MQTT_DBG("topick   :\"%s\"\r\n",topick);

	}
	else
		goto exit;

	/* loop getting msgs on subscribed topic */
	topicString.cstring = topick;
//	while (1)
//	{
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
			MQTT_DBG("message  :\" %.*s\"\r\n", payloadlen_in, payload_in);
		}

//		MQTT_DBG("publishing reading\r\n");
//		len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
//		rc = transport_sendPacketBuffer(buf, len);
//	}

	MQTT_DBG("disconnecting\r\n");
	len = MQTTSerialize_disconnect(buf, buflen);
	rc = transport_sendPacketBuffer( buf, len);

exit:
	transport_close(mysock);
	MQTT_DBG("disconnected\r\n");
   MQTT_DBG("----------end-----------------\r\n\r\n");

	return 0;
}
