#include "string.h"
#include "mqtt_api.h"
#include "dns.h"



#define MQTT_DEBUG 1
#if MQTT_DEBUG
	#define MQTT_DBG(...) DBG(__VA_ARGS__)
#else
	#define  MQTT_DBG(...) 
#endif

void mqtt_call_back(char &qos,int &retained,char &msgid)
{
   MQTT_DBG("CALL BACK!\r\n");
}



int MQTT::begin(SOCKET p_s,uint16_t p_port)
{
    local_socket = p_s;
    local_port = p_port;
    anonymous = 0;
    MQTTPacket_connectData data1 = MQTTPacket_connectData_initializer;
    data = data1;
    transport_init(local_socket,local_port);
    last_ping_time  = millis();
    return 1;
}
/*
@return 1 for success else 0;
*/
int MQTT::set_server_domain(char *domain,uint16_t port)
{
    int ret;
    DNS dns;	
    
    server_port = port;    
    dns.begin(local_socket,local_port);
    
	MQTT_DBG("-----start----------\r\n");    
	ret = dns.query(domain);/*发送DNS请求*/
	if(ret == 1){ 
        dns.get_domain_ip(server_ip);
		MQTT_DBG("IP     : [%d.%d.%d.%d]\r\n",dns.domain_ip[0],dns.domain_ip[1],dns.domain_ip[2],dns.domain_ip[3]);
    }
	else{
        MQTT_DBG("解析错误：ret= %d\r\n",ret);
        return 0;
    }
	MQTT_DBG("------end---------\r\n\r\n",domain);    
    return 1;
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
    user_name = (char *)p_name;
    password = (char *)p_pwd;
    anonymous = 1;
}

/*
@return  1:ok;
        -1:tcp connect err,check socket,server_ip,server_port;
        -2:mqtt server no ack or ack err;
*/
int  MQTT::connect()
{
    int32_t len=0;
    int rc = 0;
    int tmp;
    uint32_t tmp_time;
    int buflen = sizeof(buf);

    /*tcp链接服务器*/
    rc = transport_open((char *)server_ip,server_port);
    if(rc == 1){
        MQTT_DBG("tcp connect server is ok !\r\n");
    }
    else{
        MQTT_DBG("tcp connect server is failed:%d\r\n",rc);
        return -1;
    }
    
    
    
    /*make link package*/
//  data.clientID.cstring = "me";
//	data.cleansession = 1;
	data.keepAliveInterval = 5;
    if(anonymous == 0){//如果不使用匿名登录，设置登录名称和密码
        data.username.cstring = user_name;
        data.password.cstring = password;
    }
    len = MQTTSerialize_connect(buf, buflen, &data); /* 1 */
    /*send package*/
    transport_sendPacketBuffer(buf,len);
    
    
    
    /* wait for connack */
    tmp_time = millis();
    do{
        tmp = MQTTPacket_read(buf, buflen, transport_getdata);
        if (tmp == CONNACK){
            unsigned char sessionPresent, connack_rc;
            if (MQTTDeserialize_connack(&sessionPresent, &connack_rc, buf, buflen) != 1 || connack_rc != 0){
                MQTT_DBG("Unable to connect, return code %d\n", connack_rc);
            }
            MQTT_DBG("connect mqtt server is ok !\r\n");
            return 1;
        }
    }while(tmp != CONNACK && (millis() - tmp_time < 1000));
    return -2;
}
int  MQTT::connected()
{
    return transport_connnected();
}

/*
@return  1:ok;
*/
int MQTT::disconnect()
{
  int32_t len=0;
  int rc = 0;
  int buflen = sizeof(buf);
    
  len = MQTTSerialize_disconnect(buf, buflen ); 
  rc = transport_sendPacketBuffer(buf,len);
    
  transport_close(local_socket);

  return 1;
}
/*
@return  1:ok;
        -1:send topick failed;
        -2:publish failed;
*/
int MQTT::publish(char *topick,char *message)
{
    int32_t len=0;
    int rc = 0;
    int buflen = sizeof(buf);  
    MQTTString topicString = MQTTString_initializer;
    int msglen = strlen(message);


    topicString.cstring = topick;


    MQTT_DBG("----------start:publish-----------------\r\n");


    //组织消息
    len = MQTTSerialize_publish(buf, buflen, 0, 0, 0, 0, topicString, (unsigned char*)message, msglen); /* 2 */
    rc = transport_sendPacketBuffer(buf,len);
    if(rc > 0){
        MQTT_DBG("topick  : %s\r\n",topick);
        MQTT_DBG("message : %s\r\n",message);
        MQTT_DBG("trans is ok,len = %d\r\n",rc);
    }
    else{
        MQTT_DBG("trans is failed,len = %x\r\n",rc);
        MQTT_DBG("---------------------------\r\n");
        return -1;
    }
    if (rc == len){
        MQTT_DBG("Successfully published\r\n");
    }
    else{
        MQTT_DBG("Publish failed\r\n");
        return -2;
    }
    MQTT_DBG("-----------end------------------\r\n\r\n");
    return 1;
}

int MQTT::subscribe(char *topick)
{
    int32_t len=0;
    int rc = 0;
    int tmp = 0;
    uint32_t tmp_time;
    int buflen = sizeof(buf);  
	int msgid = 1;
	int req_qos = 1;

	char* payload = "mypayload";
    MQTTString topicString = MQTTString_initializer;
	int payloadlen = strlen(payload);
    topicString.cstring = topick;


	/* subscribe */
	topicString.cstring = topick;
	len = MQTTSerialize_subscribe(buf, buflen, 0, msgid, 1, &topicString, &req_qos);

	rc = transport_sendPacketBuffer(buf, len);
    tmp_time = millis();
    do{
        tmp = MQTTPacket_read(buf, buflen, transport_getdata);
        if (tmp == SUBACK) 	/* wait for suback */
        {
            unsigned short submsgid;
            int subcount;
            int granted_qos;

            rc = MQTTDeserialize_suback(&submsgid, 1, &subcount, &granted_qos, buf, buflen);
            if (granted_qos != 0)
            {
                MQTT_DBG("granted qos != 0, %d\n", granted_qos);
            }
            MQTT_DBG("subscribe:ok!\r\n");
            MQTT_DBG("topick   :\"%s\"\r\n",topick);
            MQTT_DBG("submsgid:%d\r\n",submsgid);
            MQTT_DBG("subcount:%d\r\n",subcount);
            MQTT_DBG("granted_qos:%d\r\n",granted_qos);

        }
    }while(tmp != SUBACK && (millis() - tmp_time < 1000));
    
    do{
        tmp = MQTTPacket_read(buf, buflen, transport_getdata);
        if (tmp == PUBLISH){
            MQTT_DBG("first req\r\n");
        }
    }while(tmp != PUBLISH);

	return 0;
}

void MQTT::loop()
{
    uint8_t ping_buf[2];
	int     ping_buf_len = sizeof(ping_buf);
    int     ping_len = MQTTSerialize_pingreq(ping_buf,ping_buf_len);
    int     buflen = sizeof(buf);  

     if(millis() - last_ping_time > 5000){
        last_ping_time = millis();
        transport_sendPacketBuffer(ping_buf, ping_len);
        MQTT_DBG("send ping\r\n");
    }
    if(callback){
        /* transport_getdata() has a built-in 1 second timeout,
        your mileage will vary */
        int tmp = MQTTPacket_read(buf, buflen, transport_getdata);
        if (tmp == PUBLISH){
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
            if(rc == 1){
        //            MQTT_DBG("message  :\" %.*s\"\r\n", payloadlen_in, payload_in);
                callback(receivedTopic.lenstring.data,receivedTopic.lenstring.len,(char*)payload_in,payloadlen_in);
            }
            else
                MQTT_DBG("message  err :%d",rc);                
        }
        else if(tmp == PINGRESP){
            MQTT_DBG("recv ping\r\n");
        }
        else{
             //MQTT_DBG("other pack! %d\r\n",tmp);
        }
    }
}
MQTT& MQTT::setCallback(void(*callback)(char*,int, char*, int)){
    this->callback = callback;
    return *this;
}
