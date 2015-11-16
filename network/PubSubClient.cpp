/*
  PubSubClient.cpp - A simple client for MQTT.
  Nick O'Leary
  http://knolleary.net
*/

#include "PubSubClient.h"
#include "string.h"

PubSubClient::PubSubClient(TCPCLIENT* p_tcp,SOCKET p_s,uint16_t p_local_port)
{
	tcp = p_tcp;
	s	= p_s;
	local_port = p_local_port;
}
void PubSubClient::begin(uint8_t *server_ip,uint16_t server_port)
{
	tcp->begin(s,local_port);
	memcpy(rip,server_ip,4);
	rport = server_port;
	
}
bool PubSubClient::connect(const char *id)
{
    return connect(id,NULL,NULL,0,0,0,0);
}

bool PubSubClient::connect(const char *id, const char *user, const char *pass, const char* willTopic, uint8_t willQos, bool willRetain, const char* willMessage)
{
            uint16_t len;
	if(!connected()){
		int result = 0;
		result = tcp->connect(rip,rport);
        if (result) {
            nextMsgId = 1;
            // Leave room in the buffer for header and variable length field
            uint16_t length = 5;
            unsigned int j;

#if MQTT_VERSION == MQTT_VERSION_3_1
            uint8_t d[9] = {0x00,0x06,'M','Q','I','s','d','p', MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 9
#elif MQTT_VERSION == MQTT_VERSION_3_1_1
            uint8_t d[7] = {0x00,0x04,'M','Q','T','T',MQTT_VERSION};
#define MQTT_HEADER_VERSION_LENGTH 7
#endif
            for (j = 0;j<MQTT_HEADER_VERSION_LENGTH;j++) {
                buffer[length++] = d[j];
            }

            uint8_t v;
            if (willTopic) {
                v = 0x06|(willQos<<3)|(willRetain<<5);
            } else {
                v = 0x02;
            }

            if(user != NULL) {
                v = v|0x80;

                if(pass != NULL) {
                    v = v|(0x80>>1);
                }
            }

            buffer[length++] = v;

            buffer[length++] = ((MQTT_KEEPALIVE) >> 8);
            buffer[length++] = ((MQTT_KEEPALIVE) & 0xFF);
            length = writeString(id,buffer,length);
            if (willTopic) {
                length = writeString(willTopic,buffer,length);
                length = writeString(willMessage,buffer,length);
            }

            if(user != NULL) {
                length = writeString(user,buffer,length);
                if(pass != NULL) {
                    length = writeString(pass,buffer,length);
                }
            }

            write(MQTTCONNECT,buffer,length-5);

            lastInActivity = lastOutActivity = millis();
			
            while (!tcp->available()) {
                unsigned long t = millis();
                if (t-lastInActivity > MQTT_KEEPALIVE*1000UL) {
                    state = MQTT_CONNECTION_TIMEOUT;
                    tcp->stop();
                    return false;
                }
            }
            uint8_t llen;
			len = readPacket(&llen);

            if (len == 4) {
                if (buffer[3] == 0) {
                    lastInActivity = millis();
                    pingOutstanding = false;
                    state = MQTT_CONNECTED;
                    return true;
                } else {
                    state = buffer[3];
                }
            }
            tcp->stop();
        } else {
            state = MQTT_CONNECT_FAILED;
        }
        return false;
    }
    return true;
}
bool PubSubClient::write(uint8_t header, uint8_t* buf, uint16_t length) {
    uint8_t lenBuf[4];
    uint8_t llen = 0;
    uint8_t digit;
    uint8_t pos = 0;
    uint8_t rc;
    uint16_t len = length;
    do {
        digit = len % 128;
        len = len / 128;
        if (len > 0) {
            digit |= 0x80;
        }
        lenBuf[pos++] = digit;
        llen++;
    } while(len>0);

    buf[4-llen] = header;
    for (int i=0;i<llen;i++) {
        buf[5-llen+i] = lenBuf[i];
    }

#ifdef MQTT_MAX_TRANSFER_SIZE
    uint8_t* writeBuf = buf+(4-llen);
    uint16_t bytesRemaining = length+1+llen;  //Match the length type
    uint8_t bytesToWrite;
    boolean result = true;
    while((bytesRemaining > 0) && result) {
        bytesToWrite = (bytesRemaining > MQTT_MAX_TRANSFER_SIZE)?MQTT_MAX_TRANSFER_SIZE:bytesRemaining;
        rc = _client->write(writeBuf,bytesToWrite);
        result = (rc == bytesToWrite);
        bytesRemaining -= rc;
        writeBuf += rc;
    }
    return result;
#else
    rc = tcp->send(buf+(4-llen),length+1+llen);////////////////////////////////////////////
    lastOutActivity = millis();
    return (rc == 1+llen+length);
#endif
}

uint16_t PubSubClient::writeString(const char* string, uint8_t* buf, uint16_t pos) {
	const char* idp = string;
	uint16_t i = 0;
	pos += 2;
	while (*idp) {
		buf[pos++] = *idp++;
		i++;
	}
	buf[pos-i-2] = (i >> 8);
	buf[pos-i-1] = (i & 0xFF);
	return pos;
}

bool PubSubClient::connected()
{
	int rc;
	if(tcp == NULL)
		rc = false;
	else
	{
		rc = tcp->is_connected();
		if(!rc)
		{
			if(state == MQTT_CONNECTED)
			{
				state = MQTT_CONNECTION_LOST;
				tcp->stop();
			}
		}
	}
	return rc;
}

bool PubSubClient::subscribe(const char* topic) {
    return subscribe(topic, 0);
}
bool PubSubClient::subscribe(const char* topic, uint8_t qos) {
    if (qos < 0 || qos > 1) {
        return false;
    }
    if (MQTT_MAX_PACKET_SIZE < 9 + strlen(topic)) {
        // Too long
        return false;
    }
    if (connected()) {
        // Leave room in the buffer for header and variable length field
        uint16_t length = 5;
        nextMsgId++;
        if (nextMsgId == 0) {
            nextMsgId = 1;
        }
        buffer[length++] = (nextMsgId >> 8);
        buffer[length++] = (nextMsgId & 0xFF);
        length = writeString((char*)topic, buffer,length);
        buffer[length++] = qos;
        return write(MQTTSUBSCRIBE|MQTTQOS1,buffer,length-5);
    }
    return false;
}









uint8_t mybuf[2048];
static uint16_t seek = 0;

uint8_t PubSubClient::readByte() {
return mybuf[seek++];
}

uint16_t PubSubClient::readPacket(uint8_t* lengthLength) {
    uint16_t len = 0;
	uint16_t mylen = 0;
	
	while(mylen == 0)
	{
		mylen = tcp->recv(mybuf);
	}
	
	
    buffer[len++] = readByte();
    bool isPublish = (buffer[0]&0xF0) == MQTTPUBLISH;
    uint32_t multiplier = 1;
    uint16_t length = 0;
    uint8_t digit = 0;
    uint16_t skip = 0;
    uint8_t start = 0;

	
    do {
        digit = readByte();
        buffer[len++] = digit;
        length += (digit & 127) * multiplier;
        multiplier *= 128;
    } while ((digit & 128) != 0);
    *lengthLength = len-1;

    if (isPublish) {
        // Read in topic length to calculate bytes to skip over for Stream writing
        buffer[len++] = readByte();
        buffer[len++] = readByte();
        skip = (buffer[*lengthLength+1]<<8)+buffer[*lengthLength+2];
        start = 2;
        if (buffer[0]&MQTTQOS1) {
            // skip message id
            skip += 2;
        }
    }

    for (uint16_t i = start;i<length;i++) {
        digit = readByte();
//        if (this->stream) {
//            if (isPublish && len-*lengthLength-2>skip) {
//                this->stream->write(digit);
//            }
//        }
        if (len < MQTT_MAX_PACKET_SIZE) {
            buffer[len] = digit;
        }
        len++;
    }

//    if (!this->stream && len > MQTT_MAX_PACKET_SIZE) {
//        len = 0; // This will cause the packet to be ignored.
//    }

	len = 4;
    return len;
}
