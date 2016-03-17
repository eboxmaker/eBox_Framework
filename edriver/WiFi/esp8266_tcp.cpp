/*
 * file   : esp8266_tcp.cpp
 * author : shentq&link
 * version: V1.0
 * date   : 2016/03/18
 *
 * esp8266 library for eBOX.
 *
 * Copyright Notice
 *    No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq and link.
 * Disclaimer
 *    This specification is preliminary and is subject to change at any time without notice. shentq and link assumes no responsibility for any errors contained herein.
 *
 *
 * Modification History:
 * -Link&shentq             - Version 0.1 (2016/3/18)
 */
 
 #include "esp8266_tcp.h"

#define TCP_DEBUG(...) uart1.printf(__VA_ARGS__)

WIFI_TCP::WIFI_TCP(ESP8266 *wifi)
{
    this->wifi = wifi;
}

/**
	* begin connect to AP . 
	* 
	* @param SSID - the SSID of AP. 
	* @param PASSWORD - the PASSWORD of AP.
	* @retval true - success.
	* @retval false - failure.
*/
bool WIFI_TCP::begin(char *SSID,char *PASSWORD)
{
    bool ret;
    ret = wifi->set_opr_to_station();
    if(ret == true)
        TCP_DEBUG("set sta ok!\r\n");
    else
        TCP_DEBUG("set sta fail!\r\n");
    TCP_DEBUG("pleas wait, join wifi will take about 10 seconds.\r\n");
    ret = wifi->join_AP((char *)SSID,(char *)PASSWORD);
    if(ret == true)
        TCP_DEBUG("join ap ok!\r\n");
    else
        TCP_DEBUG("join ap failed!\r\n");
    
    return ret;
}
/**
	* connect to remote server or client . 
	* 
	* @param remote_ip - the ip of remote server or client . 
	* @param remote_port - the port of remote server or client .
	* @param local_port - the port of local server or client .
	* @retval true - success.
	* @retval false - failure.
*/
bool WIFI_TCP::connect(char *remote_ip, uint32_t remote_port,uint32_t local_port)
{
    return wifi->create_TCP(remote_ip,remote_port,local_port);
}

/**
	* tcp send data. 
	* 
	* @param buf - buffer to send . 
	* @param len - length to send . 
	* @retval >0 - send length.
	* @retval 0 - failure.
*/

uint16_t WIFI_TCP::send(uint8_t *buf,uint16_t len)
{
    bool ret;
    ret = wifi->send(buf,len);
    if(ret)
       ;//uart1.printf("send ok!\r\n");
    else
		{  
			;//uart1.printf("send fail!\r\n");
			len = 0;
		}
    return len;
}

/**
	* tcp read data. 
	* 
	* @param buf - buffer to read . 
	* @retval >0 - read length.
	* @retval 0 - failure.
*/

uint16_t WIFI_TCP::read(char *buf)
{
    uint16_t len = 0;
    len = wifi->read((unsigned char *)buf);

    return len;
    
}

/**
	* connect to remote server or client . 
	* 
	* @param mux_id - the identifier of this TCP(available value: 0 - 4). 
	* @param remote_ip - the ip of remote server or client . 
	* @param remote_port - the port of remote server or client .
	* @param local_port - the port of local server or client .
	* @retval true - success.
	* @retval false - failure.
*/

bool WIFI_TCP::connect(uint8_t mux_id, char *remote_ip, uint32_t remote_port,uint32_t local_port)
{
    return wifi->create_TCP(mux_id,remote_ip,remote_port,local_port);
}

/**
	* tcp send data. 
	* 
	* @param mux_id - the identifier of this TCP(available value: 0 - 4). 
	* @param buf - buffer to send . 
	* @param len - length to send . 
	* @retval >0 - send length.
	* @retval 0 - failure.
*/
uint16_t WIFI_TCP::send(uint8_t mux_id,uint8_t *buf,uint16_t len)
{
    bool ret;
    ret = wifi->send(mux_id,buf,len);
    if(ret)
      ;// uart1.printf("send ok!\r\n");
    else
		{  
			;//uart1.printf("send fail!\r\n");
			len = 0;
		}
    return len;
}


/**
	* tcp read data. 
	*
	* @param mux_id - the identifier of this TCP(available value: 0 - 4). 
	* @param buf - buffer to read . 
	* @retval >0 - read length.
	* @retval 0 - failure.
*/
uint16_t WIFI_TCP::read(uint8_t *mux_id,char *buf)
{
    uint16_t len = 0;
    len = wifi->read(mux_id,(unsigned char *)buf);
    return len;
    
}


/**
	* tcp read one byte from ringbuffer.
	* @retval data
	* @note before read  call wifi.available() to check first.
*/
uint8_t WIFI_TCP::read_onebyte(void)
{    
    return 0;
//    return wifi->net_buf.read();
   
}

