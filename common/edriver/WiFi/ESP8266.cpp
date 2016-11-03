/*
 * file   : esp8266.cpp
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




#include "ESP8266.h"
#include <string.h>
#include <stdlib.h>


char ssid[] = "DVmaster";
char password[] = "dvmaster456";


#if 1
#define ESP_DEBUG(...) uart1.printf("[ESP]"),uart1.printf(__VA_ARGS__)
#else
#define  ESP_DEBUG(...)
#endif

ESP8266 wifi;



NET_DATA_STATE_T net_data_state = NEED_I;
static uint8_t net_temp_data_rx_buf[8];

/* +IPD,<id>,<len>:<data> */
/* +IPD,<len>:<data> */
void net_data_state_process(char c)
{
    switch((uint8_t)net_data_state)
    {
    case (uint8_t)NEED_PLUS:
        if(c == '+')
            net_data_state = NEED_I;
        else
            net_data_state = NEED_PLUS;
        break;
    case (uint8_t)NEED_I:
        if(c == 'I')
            net_data_state = NEED_P;
        else
            net_data_state = NEED_PLUS;
        break;
    case (uint8_t)NEED_P:
        if(c == 'P')
            net_data_state = NEED_D;
        else
            net_data_state = NEED_PLUS;
        break;
    case (uint8_t)NEED_D:
        if(c == 'D')
            net_data_state = NEED_DOT;
        else
            net_data_state = NEED_PLUS;
        break;
    case (uint8_t)NEED_DOT:
        if(c == ',')
            net_data_state = NEED_LEN_DATA;
        else
            net_data_state = NEED_PLUS;
        break;
    case (uint8_t)NEED_LEN_DATA:
        if(c >= '0' && c <= '9')
        {
            net_temp_data_rx_buf[wifi.net_data_rx_cnt++] = c;
        }
        else if(c == ':')/*<len>:<data>*/
        {
            net_temp_data_rx_buf[wifi.net_data_rx_cnt++] = '\0';
            wifi.net_data_len = atoi((const char *)net_temp_data_rx_buf);
            net_data_state = NEED_USER_DATA;
            wifi.net_data_rx_cnt = 0;
        }
        else if(c == ',') /* +IPD,<id>,<len>:<data> */
        {
            wifi.net_data_id = net_temp_data_rx_buf[0] - 48; //char to int
            wifi.net_data_rx_cnt = 0;
        }
        else
        {
            wifi.net_data_rx_cnt = 0;
            net_data_state = NEED_PLUS;
        }
        break;
    case (uint8_t)NEED_COLON:
        if(c == ':')
            net_data_state = NEED_USER_DATA;
        else
            net_data_state = NEED_PLUS;
        break;
    //        case (uint8_t)NEED_ID_DATA:break;
    case (uint8_t)NEED_USER_DATA:
        if(wifi.net_data_rx_cnt < wifi.net_data_len)
        {
            wifi.net_buf.write(c);
            wifi.net_data_rx_cnt++;
        }
        else
        {
            wifi.net_data_rx_cnt = 0;
            net_data_state = NEED_PLUS;
        }
        break;
    default:
        net_data_state = NEED_PLUS;
        wifi.net_data_rx_cnt = 0;
        break;
    }


}
/*
 * uart rx callback.
 */
void ESP8266::get_char(void)
{
    char c;
    c = uart->read();
    last_time = millis();
    if(wifi_mode == TRANSPARENT_MODE)
    {
        wifi.net_buf.write(c);
        return;
    }
    else if(wifi_mode == CMD_MODE)
    {
        rx_cmd_buf[rx_cnt] = c;
        if(rx_cnt++ > RX_BUFFER_SIZE)
        {
            rx_cnt = 0;
        }
        cmd_state = RECEIVING;
    }
    net_data_state_process(c);
}

void uart_interrupt_event(void)
{
    wifi.get_char();
}

/*
 * Empty the buffer or UART RX.
 */
void ESP8266::clear_rx_cdm_buffer(void)
{
    uint16_t i;
    for(i = 0; i < RX_BUFFER_SIZE; i++)
    {
        rx_cmd_buf[i] = 0;
    }
    rx_cnt = 0;
}

/*
 * begin
 *
 * @param uart - an reference of HardwareSerial object.
 * @param baud - the buad rate to communicate with ESP8266(default:9600).
 *
 * @warning parameter baud depends on the AT firmware. 9600 is an common value.
 */
bool ESP8266::begin(Gpio *rst, Uart *uart, uint32_t baud)
{
    bool ret;
    this->rst   = rst;
    this->uart  = uart;

    this->uart->begin(baud);
//    this->uart->attach_rx_interrupt(uart_interrupt_event);
	  this->uart->attach(uart_interrupt_event,RxIrq);
    this->rst->mode(OUTPUT_PP);

    wifi_mode = NET_MODE;

    hard_reset();

    net_buf.begin(net_data_buf, NET_DATA_BUFFER_SIZE); //��ʼ�����λ�����
    net_data_rx_cnt = 0;
    net_data_len = 0;
    net_data_id = 0;

    clear_rx_cdm_buffer();//���AT������ջ�����
    ret = restart();
    ret = kick();
    delay_ms(1000);
    return ret;
}
/**
 * hardware reset
 *
 *
 *
 */
void ESP8266::hard_reset()
{
    rst->reset();
    delay_ms(100);
    rst->set();
    delay_ms(500);
}

bool ESP8266::join_ap(char *ssid, char *pwd)
{
    bool ret;
    ret = set_opr_to_station();
    if(ret == true)
        ESP_DEBUG("set sta ok!\r\n");
    else
        ESP_DEBUG("set sta fail!\r\n");
    ESP_DEBUG("pleas wait, join wifi will take about 10 seconds.\r\n");
    ret = join_AP((char *)ssid, (char *)pwd);
    if(ret == true)
        ESP_DEBUG("join ap ok!\r\n");
    else
        ESP_DEBUG("join ap failed!\r\n");
    return ret;
}
bool ESP8266::join_ap()
{
    bool ret;
    ret = set_opr_to_station();
    if(ret == true)
        ESP_DEBUG("set sta ok!\r\n");
    else
        ESP_DEBUG("set sta fail!\r\n");
    ESP_DEBUG("pleas wait, join wifi will take about 10 seconds.\r\n");
    ret = join_AP(ssid, password);
    if(ret == true)
        ESP_DEBUG("join ap ok!\r\n");
    else
        ESP_DEBUG("join ap failed!\r\n");
    return ret;
}
/**
 * Verify ESP8266 whether live or not.
 *
 * Actually, this method will send command "AT" to ESP8266 and waiting for "OK".
 *
 * @retval true - alive.
 * @retval false - dead.
 */
bool ESP8266::kick(void)
{
    return exc_AT();
}

/**
 * Restart ESP8266 by "AT+RST".
 *
 * This method will take 3 seconds or more.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::restart(void)
{
    if (exc_AT_RST())
    {
        if (exc_AT())
        {
            return true;
        }
    }
    return false;
}


/**
 * Get the version of AT Command Set.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::get_version(char *version)
{
    return exc_AT_GMR(version);
}



/**
 * Set operation mode to staion.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::set_opr_to_station(void)
{
    uint8_t mode;
    if (!query_AT_CWMODE(&mode))
    {
        return false;
    }
    if (mode == 1)
    {
        return true;
    }
    else
    {
        if (set_AT_CWMODE(1) && restart())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}


/**
 * Set operation mode to softap.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::set_opr_to_softAP(void)
{
    uint8_t mode;
    if (!query_AT_CWMODE(&mode))
    {
        return false;
    }
    if (mode == 2)
    {
        return true;
    }
    else
    {
        if (set_AT_CWMODE(2) && restart())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

/**
 * Set operation mode to station + softap.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::set_opr_to_stationSoftAP(void)
{
    uint8_t mode;
    if (!query_AT_CWMODE(&mode))
    {
        return false;
    }
    if (mode == 3)
    {
        return true;
    }
    else
    {
        if (set_AT_CWMODE(3) && restart())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}
/**
 * Search available AP list and return it.
 *
 * @return the list of available APs.
 * @note This method will occupy a lot of memeory(hundreds of Bytes to a couple of KBytes).
 *  Do not call this method unless you must and ensure that your board has enough memery left.
 */
bool ESP8266::get_APList(char *list)
{
    return exc_AT_CWLAP(list);
}
/**
 * Join in AP.
 *
 * @param ssid - SSID of AP to join in.
 * @param pwd - Password of AP to join in.
 * @retval true - success.
 * @retval false - failure.
 * @note This method will take a couple of seconds.
 */
bool ESP8266::join_AP(char *ssid, char *pwd)
{
    return set_AT_CWJAP(ssid, pwd);
}

/**
 * Leave AP joined before.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::leave_AP(void)
{
    return exc_AT_CWQAP();
}

/**
 * Get the IP address of ESP8266.
 * @retval true - success.
 * @retval false - failure.
 */

bool ESP8266::query_sta_ip(char *msg)
{
    bool ret;
    char buf[128];
    ret = wifi.query_AT_CIPSTA(buf);
    if(ret)
    {
        wifi.get_str(buf, "\"", 1, "\"", 2, msg);
    }
    return ret;
}
/**
 * Get the IP gateway of ESP8266.
 * @retval true - success.
 * @retval false - failure.
 */

bool ESP8266::query_sta_gateway(char *msg)
{
    bool ret;
    char buf[128];
    ret = wifi.query_AT_CIPSTA(buf);
    if(ret)
    {
        wifi.get_str(buf, "\"", 3, "\"", 4, msg);
    }
    return ret;
}
/**
 * Get the IP netmask of ESP8266.
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::query_sta_netmask(char *msg)
{
    bool ret;
    char buf[128];
    ret = wifi.query_AT_CIPSTA(buf);
    if(ret)
    {
        wifi.get_str(buf, "\"", 5, "\"", 6, msg);
    }
    return ret;
}
/**
 * Set SoftAP parameters.
 *
 * @param ssid - SSID of SoftAP.
 * @param pwd - PASSWORD of SoftAP.
 * @param chl - the channel (1 - 13, default: 7).
 * @param ecn - the way of encrypstion (0 - OPEN, 1 - WEP,
 *  2 - WPA_PSK, 3 - WPA2_PSK, 4 - WPA_WPA2_PSK, default: 4).
 * @note This method should not be called when station mode.
 */
bool ESP8266::set_SoftAP_param(char *ssid, char *pwd, uint8_t chl, uint8_t ecn)
{
    return set_AT_CWSAP(ssid, pwd, chl, ecn);
}


/**
 * Get the IP list of devices connected to SoftAP.
 *
 * @return the list of IP.
 * @note This method should not be called when station mode.
 */
bool ESP8266::get_joined_DeviceIP(char *list)
{
    return exc_AT_CWLIF(list);

}

/**
 * Get the current status of connection(UDP and TCP).
 *
 * @return the status.
 */
WIFI_STATUS_T ESP8266::get_IP_status()
{
    char buf[100];
    char status;
    if( exc_AT_CIPSTATUS(buf)){
        if(search_str(buf, "STATUS:") != -1){
            get_str(buf, "STATUS:", 1, "\r\n", 1, &status);
            status -= 0x30; 
        }
        else
            status = 0;

    }      
    return (WIFI_STATUS_T)status;    
}

/**
 * Get the IP address of ESP8266.
 *
 * @return the IP list.
 */
bool ESP8266::get_local_IP(char *list)
{
    return exc_AT_CIFSR(list);
}
/**
 * Enable IP MUX(multiple connection mode).
 *
 * In multiple connection mode, a couple of TCP and UDP communication can be builded.
 * They can be distinguished by the identifier of TCP or UDP named mux_id.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::enable_MUX(void)
{
    return set_AT_CIPMUX(1);
}
/**
 * Disable IP MUX(single connection mode).
 *
 * In single connection mode, only one TCP or UDP communication can be builded.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::disable_MUX(void)
{
    return set_AT_CIPMUX(0);
}

/**
 * Create TCP connection in single mode.
 *
 * @param addr - the IP or domain name of the target host.
 * @param port - the port number of the target host.
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::create_TCP(char *addr, uint32_t port, uint32_t loca_port)
{
    return set_AT_CIPSTART_single("TCP", addr, port, loca_port);
}
/**
 * Release TCP connection in single mode.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::release_TCP(void)
{
    return exc_AT_CIPCLOSE_single();
}

/**
* Register UDP port number in single mode.
*
* @param addr - the IP or domain name of the target host.
* @param port - the port number of the target host.
* @retval true - success.
* @retval false - failure.
*/
bool ESP8266::register_UDP(char *addr, uint32_t port, uint32_t loca_port)
{
    return set_AT_CIPSTART_single("UDP", addr, port, loca_port);
}

/**
 * Unregister UDP port number in single mode.
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::unregister_UDP(void)
{
    return exc_AT_CIPCLOSE_single();
}

/**
 * Create TCP connection in multiple mode.
 *
 * @param mux_id - the identifier of this TCP(available value: 0 - 4).
 * @param addr - the IP or domain name of the target host.
 * @param port - the port number of the target host.
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::create_TCP(uint8_t mux_id, char *addr, uint32_t port, uint32_t loca_port)
{
    return set_AT_CIPSTART_multiple(mux_id, "TCP", addr, port, loca_port);
}
/**
 * Release TCP connection in multiple mode.
 *
 * @param mux_id - the identifier of this TCP(available value: 0 - 4).
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::release_TCP(uint8_t mux_id)
{
    return set_AT_CIPCLOSE_mulitple(mux_id);
}

/**
 * Register UDP port number in multiple mode.
 *
 * @param mux_id - the identifier of this TCP(available value: 0 - 4).
 * @param addr - the IP or domain name of the target host.
 * @param port - the port number of the target host.
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::register_UDP(uint8_t mux_id, char *addr, uint32_t port, uint32_t loca_port)
{
    return set_AT_CIPSTART_multiple(mux_id, "UDP", addr, port, loca_port);
}
/**
* Unregister UDP port number in multiple mode.
*
* @param mux_id - the identifier of this TCP(available value: 0 - 4).
* @retval true - success.
* @retval false - failure.
*/
bool ESP8266::unregister_UDP(uint8_t mux_id)
{
    return set_AT_CIPCLOSE_mulitple(mux_id);
}
/**
 * Set the timeout of TCP Server.
 *
 * @param timeout - the duration for timeout by second(0 ~ 28800, default:180).
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::set_TCPServer_timeout(uint32_t timeout)
{
    return set_AT_CIPSTO(timeout);
}

/**
 * Start TCP Server(Only in multiple mode).
 *
 * After started, user should call method: getIPStatus to know the status of TCP connections.
 * The methods of receiving data can be called for user's any purpose. After communication,
 * release the TCP connection is needed by calling method: releaseTCP with mux_id.
 *
 * @param port - the port number to listen(default: 333).
 * @retval true - success.
 * @retval false - failure.
 *
 * @see String getIPStatus(void);
 * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
 * @see bool releaseTCP(uint8_t mux_id);
 */
bool ESP8266::start_TCPServer(uint32_t port)
{
    if (set_AT_CIPSERVER(1, port))
    {
        return true;
    }
    return false;
}

/**
* Stop TCP Server(Only in multiple mode).
*
* @retval true - success.
* @retval false - failure.
*/
bool ESP8266::stop_TCPServer(void)
{
    set_AT_CIPSERVER(0);
    restart();
    return false;
}

/**
 * Start Server(Only in multiple mode).
 *
 * @param port - the port number to listen(default: 333).
 * @retval true - success.
 * @retval false - failure.
 *
 * @see String getIPStatus(void);
 * @see uint32_t recv(uint8_t *coming_mux_id, uint8_t *buffer, uint32_t len, uint32_t timeout);
 */
bool ESP8266::start_Server(uint32_t port)
{
    return start_TCPServer(port);
}

/**
 * Stop Server(Only in multiple mode).
 *
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::stop_Server(void)
{
    return stop_TCPServer();
}

/**
 * Send data based on TCP or UDP builded already in single mode.
 *
 * @param buffer - the buffer of data to send.
 * @param len - the length of data to send.
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::send(const uint8_t *buffer, uint32_t len)
{
    return set_AT_CIPSEND_single(buffer, len);
}

/**
 * Send data based on one of TCP or UDP builded already in multiple mode.
 *
 * @param mux_id - the identifier of this TCP(available value: 0 - 4).
 * @param buffer - the buffer of data to send.
 * @param len - the length of data to send.
 * @retval true - success.
 * @retval false - failure.
 */
bool ESP8266::send(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    return set_AT_CIPSEND_multiple(mux_id, buffer, len);
}

/**
 *bool ESP8266::exc_AT(void)

 *@breif    AT��������
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT(void)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::exc_AT_RST(void)

 *@breif    ���͸�λ����
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_RST(void)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+RST\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    return ret;
}
/**
 *bool ESP8266::exc_AT_GMR(char *version)

 *@breif    ��ѯesp8266�汾��
 *@param    buf:�������ݻ�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_GMR(char *msg)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+GMR\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\nOK", 1, msg);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}
bool ESP8266::exc_AT_GSLP(uint8_t time)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    wifi_mode = CMD_MODE;
    clear_rx_cdm_buffer();
    uart->printf("AT+GSLP=%d\r\n", time);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}
bool ESP8266::exc_AT_ATE(const char *cmd)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("%S\r\n", cmd);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}
bool ESP8266::exc_AT_RESTORE(void)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+RESTORE\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    return ret;
}
bool ESP8266::exc_AT_UART(uint32_t baud_rate, uint8_t data_bits, uint8_t stop_bits, uint8_t parity, uint8_t control)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+UART=%d,%d,%d,%d,%d\r\n", baud_rate, data_bits, stop_bits, parity, control);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::query_AT_CWMODE(uint8_t *mode)

 *@breif    ��ѯWiFiӦ��ģʽ
 *@param    mode:WiFiģʽ�ķ���ֵ���û�Ӧ����ĳ��������ָ��
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::query_AT_CWMODE(uint8_t *mode)
{
    bool ret = false;
    char str_mode[4];
    if (!mode)
    {
        return false;
    }
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWMODE?\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "+CWMODE:", 1, "\r\n\r\nOK", 1, str_mode);
            *mode = (uint8_t)atoi(str_mode);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CWMODE(uint8_t mode)

 *@breif    ����WiFiӦ��ģʽ
 *@param    mode:WiFiģʽ��ֵ
                1��stationģʽ
                2��APģʽ
                3��AP��stationģʽ
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWMODE(uint8_t mode)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWMODE=%d\r\n", mode);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //return recv_find_str("OK", "no change");
}


/**
 *bool ESP8266::exc_AT_CWLAP(char *list)

 *@breif    �г���ǰ����AP
 *@param    list:AP�б�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CWLAP(char *list)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWLAP\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\nOK", 1, list);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CWJAP(char *ssid, char *pwd)

 *@breif    ����(AP)·��
 *@param    ssid:·�ɵ�ssid�ַ���
 *@param    pwd:·�ɵ�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWJAP(char *ssid, char *pwd)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWJAP=\"%s\",\"%s\"\r\n", ssid, pwd);

    if(wait_cmd(10000) == RECEIVED)
    {
        if (search_str(rx_cmd_buf, "OK") != -1)
        {
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::exc_AT_CWQAP(void)

 *@breif    �˳�AP����
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CWQAP(void)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWQAP\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CWSAP(char *ssid, char *pwd, uint8_t chl, uint8_t ecn)

 *@breif    ����APģʽ�µĲ���
 *@param    ssid��AP����������ַ���
 *@param    pwd:�����ַ������64�ֽ�
 *@param    chl:ͨ����
 *@param    ecn�����ܷ�ʽ
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWSAP(char *ssid, char *pwd, uint8_t chl, uint8_t ecn)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWSAP=\"%s\",\"%s\",%d,%d\r\n", ssid, pwd, chl, ecn);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::exc_AT_CWLIF(char *list)

 *@breif    �鿴�ѽ�����豸IP��MAC�б�
 *@param    list:�����б�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CWLIF(char *list)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWLIF\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, list);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    return recv_find_filter_str("OK", "\r\r\n", "\r\n\r\nOK", list,2000);
}

/**
 *bool ESP8266::set_AT_CWDHCP(uint8_t mode,uint8_t enable)

 *@breif    DHCP���ã������ûᱣ�����û�flash�������򣬵��������ᱣ��
 *@param    mode:ģʽ
                0������AP
                1������STA
                2������AP��STA
 *@param    enable:ʹ�ܿ���
                0���ر�
                1������
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWDHCP(uint8_t mode, uint8_t enable) //DHCP����
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWDHCP=%d,%d\r\n", mode, enable);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CWAUTOCONN(uint8_t enable)

 *@breif    ����STA�����Զ�����
 *@param    enable:ʹ�ܿ���
                0����ֹ
                1������
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWAUTOCONN(uint8_t enable)     //�Զ���������
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWAUTOCONN=%d\r\n", enable);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;

}

/**
 *bool ESP8266::set_AT_CIPSTAMAC(char *mac)

 *@breif    ����STA��mac��ַ
 *@param    mac:�ַ�������������"18:fe:12:34:56:78"
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSTAMAC(char *mac)           //����STAmac
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTAMAC=%s\r\n", mac);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CIPAPMAC(char *mac)

 *@breif    ����AP��mac��ַ
 *@param    mac:�ַ�������������"18:fe:12:34:56:78"
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPAPMAC(char *mac)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPAPMAC=%s\r\n", mac);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CIPSTA(char *ip)

 *@breif    ����STA��ip
 *@param    ip: IP�ַ���"192.168.1.99"
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSTA(char *ip)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTA=%s\r\n", ip);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CIPAP(char *ip)

 *@breif    ����AP��ip
 *@param    ip: IP�ַ���"192.168.1.99"
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPAP(char *ip)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPAP=%s\r\n", ip);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CWSMARTSTART(uint8_t method)

 *@breif    �����������ӵķ���
 *@param    method:����
                0��ʹ�ð�����AI_LINK����
                1��ʹ��ESP-TOUCH����
                2��ʹ��AIR-KISS����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWSMARTSTART(uint8_t method)   //
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWSMARTSTART=%d\r\n", method);
    if(wait_cmd(10000) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::set_AT_CWSMARTSTOP(void)

 *@breif    �ر���������
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CWSMARTSTOP(void)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CWSMARTSTOP\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::query_AT_CIPSTAMAC(char *msg)

 *@breif    ��ѯSTAmac
 *@param    msg: �������ݻ�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::query_AT_CIPSTAMAC(char *msg)         //
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTAMAC?\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, msg);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::query_AT_CIPAPMAC(char *msg)

 *@breif    ��ѯAP��mac
 *@param    msg: �������ݻ�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::query_AT_CIPAPMAC(char *msg)          //
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPAPMAC?\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, msg);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::query_AT_CIPSTA(char *msg)

 *@breif    ��ѯSTA��ip
 *@param    msg: �������ݻ�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::query_AT_CIPSTA(char *msg)            //
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTA?\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, msg);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}

/**
 *bool ESP8266::query_AT_CIPAP(char *msg)

 *@breif    ��ѯAP��ip
 *@param    msg: �������ݻ�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::query_AT_CIPAP(char *msg)             //
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPAP?\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, msg);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}


/**
 *bool ESP8266::exc_AT_CIPSTATUS(char *list)

 *@breif    �鿴�ѽ�����豸IP��MAC�б�
 *@param    list:�����б�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CIPSTATUS(char *list)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTATUS\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, list);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}


/**
 *bool ESP8266::exc_AT_CIFSR(char *list)

 *@breif    ��ȡ����IP��ַ
 *@param    list:�����б�����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CIFSR(char *list)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIFSR\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
        {
            get_str(rx_cmd_buf, "\r\r\n", 1, "\r\n\r\nOK", 1, list);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    return recv_find_filter_str("OK", "\r\r\n", "\r\n\r\nOK", list,2000);
}

/**
 *bool ESP8266::set_AT_CIPMUX(uint8_t mode)

 *@breif    ���������ӹ���
 *@param    mode:������ģʽ��0����·���ӣ�1����·����
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPMUX(uint8_t mode)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPMUX=%d\r\n", mode);
    if(wait_cmd("Link is builded", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1 || search_str(rx_cmd_buf, "Link is builded") != -1) //?
        {
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    recv_find_str("OK", "Link is builded");
    //    if (index_of_str("OK") != -1) {
    //        return true;
    //    }
    //    return false;
}


/**
 *bool ESP8266::set_AT_CIPSTART_single(char *type, char *addr, uint32_t port)

 *@breif    ����TCP���ӻ���ע��UDP�˿ں�
 *@param    type:�ַ���������������������
                "TCP":����TCP����
                "UDP":����UDP����
 *@param    addr:       �ַ���������Զ�̷�����IP��ַ
 *@param    port:       Զ�̷������˿ں�
 *@param    local_port: ���ض˿ں�
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSTART_single(char *type, char *addr, uint32_t port, uint32_t loca_port)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTART=\"%s\",\"%s\",%d,%d\r\n", type, addr, port, loca_port);
    //uart->printf("AT+CIPSTART=\"%s\",\"%s\",%d\r\n",type,addr,port);

    if(wait_cmd("ALREADY CONNECT", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1 || search_str(rx_cmd_buf, "ALREADY CONNECT") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    recv_find_str("OK", "ERROR", "ALREADY CONNECT", 10000);
    //    if (index_of_str("OK") != -1 ||index_of_str("ALREADY CONNECT") != -1) {
    //        return true;
    //    }
    //    return false;
}


/**
 *bool ESP8266::set_AT_CIPSTART_multiple(uint8_t mux_id, char *type, char *addr, uint32_t port)

 *@breif    ����TCP���ӻ���ע��UDP�˿ں�
 *@param    mux_id:���ӵ�ID��
 *@param    type:�ַ���������������������
                "TCP":����TCP����
                "UDP":����UDP����
 *@param    addr:�ַ���������Զ�̷�����IP��ַ
 *@param    port:Զ�̷������˿ں�
 *@retval   �ɹ�����true��ʧ��false*/
bool ESP8266::set_AT_CIPSTART_multiple(uint8_t mux_id, char *type, char *addr, uint32_t port, uint32_t loca_port)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTART=%d,\"%s\",\"%s\",%d,%d\r\n", mux_id, type, addr, port, loca_port);
    //uart->printf("AT+CIPSTART=%d,\"%s\",\"%s\",%d\r\n",mux_id,type,addr,port);
    if(wait_cmd("ALREADY CONNECT", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1 || search_str(rx_cmd_buf, "ALREADY CONNECT") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    recv_find_str("OK", "ERROR", "ALREADY CONNECT", 10000);
    //    if (index_of_str("OK") != -1 ||index_of_str("ALREADY CONNECT") != -1) {
    //        return true;
    //    }
    //    return false;
}


/**
 *bool ESP8266::exc_AT_CIPCLOSE_single(void)

 *@breif    �ر�TCP����UDP����
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CIPCLOSE_single(void)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPCLOSE\r\n");
    if(wait_cmd("Link is not", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1 || search_str(rx_cmd_buf, "Link is not") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    return recv_find_str("OK", 2000);
}


/**
 *bool ESP8266::set_AT_CIPCLOSE_mulitple(uint8_t mux_id)

 *@breif    �ر�TCP����UDP����
 *@param    mux_id:�ر����ӵ�ID
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPCLOSE_mulitple(uint8_t mux_id)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPCLOSE=%d\r\n", mux_id);
    if(wait_cmd("Link is not", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1 || search_str(rx_cmd_buf, "Link is not") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    return recv_find_str("OK", "link is not", 5000);
}


/**
 *bool ESP8266::set_AT_CIPSTO(uint32_t timeout)

 *@breif    ����TCP��������ʱʱ��
 *@param    timeout:��ʱʱ��0~7200
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSTO(uint32_t timeout)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSTO=%d\r\n", timeout);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1)
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
    //    return recv_find_str("OK");
}


/**
 *bool ESP8266::set_AT_CIPSERVER(uint8_t mode, uint32_t port)

 *@breif    ����ΪTCP������
 *@param    mode:������ģʽ
                0���ر�
                1������
 *@param    port:�������˿ڣ�ȱʡֵΪ333��
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSERVER(uint8_t mode, uint32_t port)
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    if (mode)
    {
        clear_rx_cdm_buffer();
        uart->printf("AT+CIPSERVER=1,%d\r\n", port);
        //        return recv_find_str("OK", "no change");
    }
    else
    {
        clear_rx_cdm_buffer();
        uart->printf("AT+CIPSERVER=0\r\n");
        //        return recv_find_str("\r\r\n");
    }
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1) //��
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}


/**
 *bool ESP8266::set_AT_CIPSEND_single(const uint8_t *buffer, uint32_t len)

 *@breif    ��������
 *@param    buffer: �������ݻ�����
 *@param    len:    ���ݳ���,��󳤶�Ϊ2048
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSEND_single(const uint8_t *buffer, uint32_t len)
{
    bool ret = false;
    uint8_t state = 0;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSEND=%d\r\n", len);
    if(wait_cmd(">", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, ">") != -1 || search_str(rx_cmd_buf, "OK") != -1) //��
        {
            state = 1;
        }
        else
            ret = false;
    }else{
        ESP_DEBUG("SEND TIMEOUT!");
        ret = false;
    }

    if (state == 1)
    {
        clear_rx_cdm_buffer();
        //       uart->put_string((char *)buffer,len);
        uart->write((const char *)buffer, len);
        if(wait_cmd("SEND OK", TIMEOUT_TIME) == RECEIVED)
        {
            if(search_str(rx_cmd_buf, "SEND OK") != -1) //
            {
                ret = true;
            }
            else
                ret = false;
        }
        else
            ret = false;
        //        return recv_find_str("SEND OK", 1000);
    }
    wifi_mode = NET_MODE;
    return ret;
}



/**
 *bool ESP8266::set_AT_CIPSEND_multiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)

 *@breif    ��������
 *@param    mux_id: ����ID��
 *@param    buffer: �������ݻ�����
 *@param    len:    ���ݳ�����󳤶�Ϊ2048
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPSEND_multiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len)
{
    bool ret = false;
    uint8_t state;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPSEND=%d,%d\r\n", mux_id, len);
    if(wait_cmd(">", TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, ">") != -1 || search_str(rx_cmd_buf, "OK") != -1) //��
        {
            state = 1;
        }
        else
            ret = false;
    }

    if (state == 1)
    {
        clear_rx_cdm_buffer();
        uart->write(buffer,len);

        if(wait_cmd("SEND OK", TIMEOUT_TIME) == RECEIVED)
        {
            if(search_str(rx_cmd_buf, "SEND OK") != -1) //
            {
                ret = true;
            }
            else
                ret = false;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;
}
/**
 *bool ESP8266::set_AT_CIPMODE(uint8_t mode)

 *@breif    ����͸��ģʽ
            ����͵��ģʽ�Ὣ������TCP���ӱ�����flash user parameter����
            �´��ϵ��Ի��Զ��������Ӳ�����͸��

 *@param    mode: ͸��ģʽ
                0����͸��
                1��͸��ģʽֻ�йر�serverģʽ������Ϊ��·
 *@param    buffer: �������ݻ�����
 *@param    len:    ���ݳ���
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_CIPMODE(uint8_t mode)//
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIPMODE=%d\r\n", mode);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1) //
        {
            ret = true;
            if(mode == 1)
            {
                uart->printf("AT+CIPSEND\r\n");
                delay_ms(200);
                wait_wifi_mode(TRANSPARENT_MODE);
                return ret;
            }
        }
        else
            ret = false;
    }

    wifi_mode = NET_MODE;
    return ret;

}

/**
 *bool ESP8266::set_AT_exit_trans()//�Ƴ�͸��

 *@breif    �˳�͸��ģʽ
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::set_AT_exit_trans()//�Ƴ�͸��
{
    bool ret = true;
    uart->printf("+++");
    delay_ms(1000);
    wifi_mode = CMD_MODE;
    return ret;
}

/**
 *bool ESP8266::exc_AT_CIUPDATE(void)

 *@breif    ���������̼�
 *@param    NONE
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_CIUPDATE(void)//���������̼�
{
    bool ret = false;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+CIUPDATE\r\n");
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1) //
            ret = true;
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;

}
/**
 *bool ESP8266::exc_AT_PING(const char *host,char *msg)

 *@breif    PING��������
 *@param    host:   ����IP��������
 *@param    msg:    ping�Ľ��
 *@retval   �ɹ�����true��ʧ��false
*/
bool ESP8266::exc_AT_PING(const char *host, char *msg)
{
    bool ret;
    wait_wifi_mode(CMD_MODE);
    clear_rx_cdm_buffer();
    uart->printf("AT+PING=\"%s\"\r\n", host);
    if(wait_cmd(TIMEOUT_TIME) == RECEIVED)
    {
        if(search_str(rx_cmd_buf, "OK") != -1) //��
        {
            get_str(rx_cmd_buf, msg, rx_cnt);
            ret = true;
        }
        else
            ret = false;
    }
    wifi_mode = NET_MODE;
    return ret;

}

uint16_t ESP8266::get_str(char *source, const char *begin, uint16_t count1, const char *end, uint16_t count2, char *out)
{
    uint16_t i;
    uint16_t len1;
    uint16_t len2;
    uint16_t index1 = 0;
    uint16_t index2 = 0;
    uint16_t length = 0;
    len1 = find_str((uint8_t *)source, (uint8_t *)begin, count1, index1);
    len2 = find_str((uint8_t *)source, (uint8_t *)end, count2, index2);
    length = index2 - index1 - len1;
    if((len1 != 0) && (len2 != 0))
    {
        for( i = 0; i < index2 - index1 - len1; i++)
            out[i] = source[index1 + len1 + i];
        out[i] = '\0';
    }
    else
    {
        out[0] = '\0';
    }
    return length;

}
uint16_t ESP8266::get_str(char *source, const char *begin, uint16_t count, uint16_t length, char *out)
{
    uint16_t i = 0;
    uint16_t len1 = 0;
    uint16_t index1 = 0;
    len1 = find_str((uint8_t *)source, (uint8_t *)begin, count, index1);
    if(len1 != 0)
    {
        for(i = 0; i < length; i++)
            out[i] = source[index1 + len1 + i];
        out[i] = '\0';
    }
    else
    {
        out[0] = '\0';
    }
    return length;
}

uint16_t ESP8266::get_str(char *source, char *out, uint16_t length)
{
    uint16_t i = 0;
    for (i = 0 ; i < length ; i++)
    {
        out[i] = source[i];
    }
    out[i] = '\0';
    return length;
}
uint16_t ESP8266::find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek)
{
    uint16_t _count = 1;
    uint16_t len = 0;
    seek = 0;
    uint8_t *temp_str = NULL;
    uint8_t *temp_ptr = NULL;
    uint8_t *temp_char = NULL;
    if(0 == s_str || 0 == p_str)
        return 0;
    for(temp_str = s_str; *temp_str != '\0'; temp_str++)	 //���β����ַ���
    {
        temp_char = temp_str; //ָ��ǰ�ַ���
        //�Ƚ�
        for(temp_ptr = p_str; *temp_ptr != '\0'; temp_ptr++)
        {
            if(*temp_ptr != *temp_char)
            {
                len = 0;
                break;
            }
            temp_char++;
            len++;
        }
        if(*temp_ptr == '\0') //��������Ҫ���ҵ��ַ����˳�
        {
            if(_count == count)
                return len;
            else
            {
                _count++;
                len = 0;
            }
        }
        seek++;  //��ǰƫ������1
    }
    return 0;
}
int ESP8266::search_str(char *source, const char *target)
{
    uint16_t seek = 0;
    uint16_t len;
    len = this->find_str((uint8_t *)source, (uint8_t *)target, 1, seek);
    if(len == 0)
        return -1;
    else
        return len;

}

CMD_STATE_T ESP8266::wait_cmd(uint32_t wait_time)
{
    uint32_t time = millis();
    cmd_state = WAITING;

    while(1)
    {
        if(millis() - time > wait_time)
        {
            cmd_state = TIMEOUT;
            ESP_DEBUG("cmd TIMEOUT\r\n");
            break;
        }
        if(
            search_str(rx_cmd_buf, "OK"   ) != -1  || \
            search_str(rx_cmd_buf, "FAIL" ) != -1  || \
            search_str(rx_cmd_buf, "EEROR") != -1
        )
        {
            while(millis() - last_time < 2);//�ȴ������ֽڽ������
            cmd_state = RECEIVED;
            break;
        }
    }
//    print_cmd(cmd_state);

    return cmd_state;

}
CMD_STATE_T ESP8266::wait_cmd(const char *spacial_target, uint32_t wait_time)
{
    uint32_t time = millis();


    while(1)
    {
        if(millis() - time > wait_time)
        {
            cmd_state = TIMEOUT;
            ESP_DEBUG("cmd TIMEOUT\r\n");
            break;
        }

        if(
            search_str(rx_cmd_buf, spacial_target) != -1   || \
            search_str(rx_cmd_buf, "OK"   ) != -1          || \
            search_str(rx_cmd_buf, "FAIL" ) != -1          || \
            search_str(rx_cmd_buf, "EEROR") != -1
        )
        {
            while(millis() - last_time < 2);//�ȴ������ֽڽ������
            cmd_state = RECEIVED;
            break;
        }
    }
//    print_cmd(cmd_state);

    return cmd_state;

}
bool ESP8266::wait_wifi_mode(WIFI_MODE_T mode)
{
    if(millis() - last_time > 2)
        wifi_mode = mode;
    else
    {
        while(millis() - last_time < 2);
        wifi_mode = mode;
    }
    return true;
}

void ESP8266::print_cmd(CMD_STATE_T &cmd)
{
    if(cmd != RECEIVED)
        switch(cmd)
        {
        case WAITING:
            ESP_DEBUG("cmd WAITING\r\n");
            break;
        case RECEIVING:
            ESP_DEBUG("cmd RECEIVING\r\n");
            break;
        case RECEIVED:
            ESP_DEBUG("cmd RECVED\r\n");
            break;
        case TIMEOUT:
            ESP_DEBUG("cmd TIMEOUT\r\n");
            break;
        }
}
void ESP8266::print_rx_buf()
{
    uart1.printf("----rx_cmd_buf----\r\n%s\r\n----end----", rx_cmd_buf);
}
/**
 * check rx ringbuffer status
 *
 * @retval >0 rx ring ringbuffer length.
 * @retval <=0 failure.
 */
int  ESP8266::available()
{
    return net_buf.available();
}
/**
 * read one byte.
 * @note must Coordinate available() use;!!!!!!!!!!!!
 * @param buffer - the buffer of data to send.
 * @param buf - the buffer of data to read.
 * @retval len - the length of data readed.
 */
char ESP8266::read_one()
{
   return net_buf.read();
}
/**
 * Send data based on TCP or UDP builded already in single mode.
 *
 * @param buffer - the buffer of data to send.
 * @param buf - the buffer of data to read.
 * @retval len - the length of data readed.
 */
uint16_t ESP8266::read(unsigned char *buf)
{
    uint16_t len = available();
    if(len > 0)
    {
        for(int i = 0; i < len; i++)
        {
            buf[i] = net_buf.read();
        }
    }
    else
    {
        buf[0] = 0;
        len = 0;
    }
    return len;
}
///**
// * Send data based on TCP or UDP builded already in single mode.
// *
// * @param buffer - the buffer of data to send.
// * @param buf - the buffer of data to read.
// * @retval len - the length of data readed.
// */
//uint16_t ESP8266::read_until(unsigned char *buf,char ch)
//{
//    int i=0;
//    uint16_t len=0;
//    while(buf[i] != ch)
//    {
//        if(available()){
//            buf[i] = net_buf.read();
//            i++;
//        }
//    }
//    return len;
//}

/**
 * read data based on one of TCP or UDP builded already in multiple mode.
 *
 * @param mux_id - the identifier of this TCP(available value: 0 - 4).
 * @param buf - the buffer of data to read.
 * @retval len - the length of data readed.
 *
     */
uint16_t ESP8266::read(uint8_t *mux_id, unsigned char *buf)
{
    uint16_t len = available();
    if(len > 0)
    {
        for(int i = 0; i < len; i++)
        {
            buf[i] = net_buf.read();
        }
    }
    else
    {
        buf[0] = 0;
        len = 0;
    }
    *mux_id = net_data_id;
    return len;
}
