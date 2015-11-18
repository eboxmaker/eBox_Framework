#include "string.h"
#include "mqtt_api.h"
#include "dns.h"

#include "MQTTPacket.h"
#include "transport.h"

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
    
    
	ret = dns.query(domain);
    dns.get_domain_ip(server_ip);
	if(ret == DNS_RET_SUCCESS) /*发送DNS请求*/
		uart1.printf("Get [%s]'s IP address [%d.%d.%d.%d] from \r\n",domain,dns.domain_ip[0],dns.domain_ip[1],dns.domain_ip[2],dns.domain_ip[3]);
	else if(ret == DNS_RET_FAIL)
    uart1.printf("获取超时\r\n");
	else
    uart1.printf("未知错误.\r\n");

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
 
  data.clientID.cstring = "me";
  data.keepAliveInterval = 5;
  data.cleansession = 1;
  topicString.cstring = topick;
   //组织消息
  len = MQTTSerialize_connect(buf, buflen, &data); /* 1 */
  len += MQTTSerialize_publish(buf + len, buflen - len, 0, 0, 1, 0, topicString, (unsigned char*)message, msglen); /* 2 */
  len += MQTTSerialize_disconnect(buf + len, buflen - len); /* 3 */
 
    //链接服务器
    rc = transport_open((char *)server_ip,server_port);
  if(rc != 0)
        uart1.printf("open is ok status = %d\r\n",rc);
  else
      uart1.printf("open failed\r\n");
  //发布消息
//    while(1)
//    {
      delay_ms(500);
      rc = transport_sendPacketBuffer(buf,len);
      if(rc > 0){
            uart1.printf("---------------------------\r\n");
            uart1.printf("topick = %s\r\n",topick);
            uart1.printf("message = %s\r\n",message);
            uart1.printf("trans is ok,len = %d\r\n",rc);
      }
      else
      {
            uart1.printf("---------------------------\r\n");
             uart1.printf("trans is failed,len = %x\r\n",rc);
          //break;
      }
      
//     }
  transport_close(7);
        if (rc == len)
               uart1.printf("Successfully published\r\n");
        else
                uart1.printf("Publish failed\r\n");
  uart1.printf("-----------------------------\r\n");
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


    //链接服务器
  if(transport_open((char *)server_ip,server_port))
        uart1.printf("open is ok\r\n");

	data.clientID.cstring = "me";
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
			uart1.printf("Unable to connect, return code %d\n", connack_rc);
			goto exit;
		}
	}
	else
		goto exit;

	/* subscribe */
	topicString.cstring = "substopic";
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
			uart1.printf("granted qos != 0, %d\n", granted_qos);
			goto exit;
		}
	}
	else
		goto exit;

	/* loop getting msgs on subscribed topic */
	topicString.cstring = "pubtopic";
	while (1)
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
			uart1.printf("message arrived %.*s\r\n", payloadlen_in, payload_in);
		}

		uart1.printf("publishing reading\r\n");
		len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)payload, payloadlen);
		rc = transport_sendPacketBuffer(buf, len);
	}

	uart1.printf("disconnecting\r\n");
	len = MQTTSerialize_disconnect(buf, buflen);
	rc = transport_sendPacketBuffer( buf, len);

exit:
	transport_close(mysock);

	return 0;
}
