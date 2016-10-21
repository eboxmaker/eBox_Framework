/*
 * file   : esp8266.h
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
#ifndef __ESP8266_H__
#define __ESP8266_H__

#include "ebox.h"
#include "ringbuf.h"


#define RX_BUFFER_SIZE 1024
#define NET_DATA_BUFFER_SIZE 1024
typedef enum {
    WIFI_NO_SHIELD        = 255,   // for compatibility with WiFi Shield library
    WIFI_IDLE_STATUS      = 0,
    WIFI_NO_SSID_AVAIL    = 1,
    WIFI_SCAN_COMPLETED   = 2,
    WIFI_CONNECTED        = 3,
    WIFI_CONNECT_FAILED   = 4,
    WIFI_CONNECTION_LOST  = 5,
    WIFI_DISCONNECTED     = 6
} WIFI_STATUS_T;


typedef enum
{
    WAITING = 0,
    RECEIVING = 1,
    RECEIVED  = 2,
    TIMEOUT = 3
} CMD_STATE_T;
#define TIMEOUT_TIME 2000

typedef enum
{
    NET_MODE = 0,
    CMD_MODE = 1,
    TRANSPARENT_MODE = 2
} WIFI_MODE_T;
typedef enum
{
    NEED_PLUS = 0,
    NEED_I ,
    NEED_P ,
    NEED_D ,
    NEED_DOT ,
    NEED_ID_DATA,
    NEED_LEN_DATA,
    NEED_COLON ,
    NEED_USER_DATA ,
    STATE_ERROR ,
} NET_DATA_STATE_T;
/**
 * Provide an easy-to-use way to manipulate ESP8266.
 */
class ESP8266
{

public:
    /////��ʼ������///////////////////////////////////////////////////////////////////////
    bool begin(Gpio *rst, Uart *uart, uint32_t baud);
    void hard_reset();
    bool join_ap(char *ssid, char *pwd);
    bool join_ap();

    ////�����д����/////////////////////////////////////////////////////////////////////
    int         available();
    char        read_one();
    uint16_t	read(unsigned char *buf);//���س�����Ϣ
    uint16_t	read(uint8_t *mux_id, unsigned char *buf); //���س�����Ϣ
    bool        send(const uint8_t *buffer, uint32_t len);
    bool        send(uint8_t mux_id, const uint8_t *buffer, uint32_t len);

    ////AT�����װ��////////////////////////////////////////////////////////////////////////////////
    bool kick(void);
    bool restart(void);
    bool get_version(char *version );
    bool set_opr_to_station(void);
    bool set_opr_to_softAP(void);
    bool set_opr_to_stationSoftAP(void);
    bool get_APList(char *list);
    bool join_AP(char *ssid, char *pwd);
    bool leave_AP(void);
    bool query_sta_ip(char *msg);
    bool query_sta_gateway(char *msg);
    bool query_sta_netmask(char *msg);
    bool set_SoftAP_param(char *ssid, char *pwd, uint8_t chl = 7, uint8_t ecn = 4);
    bool get_joined_DeviceIP(char *list);
    bool get_local_IP(char *list);
    bool enable_MUX(void);
    bool disable_MUX(void);
    WIFI_STATUS_T get_IP_status();


    bool create_TCP(char *addr, uint32_t port, uint32_t loca_port);
    bool release_TCP(void);

    bool register_UDP(char *addr, uint32_t port, uint32_t loca_port);
    bool unregister_UDP(void);

    bool create_TCP(uint8_t mux_id, char *addr, uint32_t port, uint32_t loca_port);
    bool release_TCP(uint8_t mux_id);

    bool register_UDP(uint8_t mux_id, char *addr, uint32_t port, uint32_t loca_port);
    bool unregister_UDP(uint8_t mux_id);

    bool set_TCPServer_timeout(uint32_t timeout = 180);
    bool start_TCPServer(uint32_t port = 333);
    bool stop_TCPServer(void);
    bool start_Server(uint32_t port = 333);
    bool stop_Server(void);
    /////AT����//////////////////////////////////////////////////////////////////////////////////////////////
private:
    //����ATָ��
    bool exc_AT(void);
    bool exc_AT_RST(void);
    bool exc_AT_GMR(char *msg);
    bool exc_AT_GSLP(uint8_t time);
    bool exc_AT_ATE(const char *cmd);
    bool exc_AT_RESTORE(void);
    bool exc_AT_UART(uint32_t baud_rate, uint8_t data_bits, uint8_t stop_bits, uint8_t parity, uint8_t control);
    //WiFi����ָ��
    bool query_AT_CWMODE(uint8_t *mode);        //��ѯ��ǰWiFiӦ��ģʽ
    bool set_AT_CWMODE(uint8_t mode);           //����WiFiӦ��ģʽ
    bool exc_AT_CWLAP(char *list);              //�г���ǰ����AP
    bool set_AT_CWJAP(char *ssid, char *pwd);   //����AP
    bool exc_AT_CWQAP(void);                    //�˳�AP������
    bool set_AT_CWSAP(char *ssid, char *pwd, uint8_t chl, uint8_t ecn);//����APģʽ�µĲ���
    bool exc_AT_CWLIF(char *list);              //�鿴�����ѽ����豸��IP
    bool set_AT_CWDHCP(uint8_t mode, uint8_t enable); //DHCP����
    bool set_AT_CWAUTOCONN(uint8_t enable);     //�Զ���������
    bool set_AT_CIPSTAMAC(char *mac);           //����STAmac
    bool set_AT_CIPAPMAC(char *mac);            //����AP��mac
    bool set_AT_CIPSTA(char *ip);               //����STA��ip
    bool set_AT_CIPAP(char *ip);                //����AP��ip
    bool set_AT_CWSMARTSTART(uint8_t method);   //�����������ӵķ���
    bool set_AT_CWSMARTSTOP(void);              //�ر���������
    bool query_AT_CIPSTAMAC(char *msg);         //��ѯSTAmac
    bool query_AT_CIPAPMAC(char *msg);          //��ѯAP��mac
    bool query_AT_CIPSTA(char *msg);            //��ѯSTA��ip
    bool query_AT_CIPAP(char *msg);             //��ѯAP��ip

    //TCP/IP������ATָ��
    bool exc_AT_CIPSTATUS(char *list);
    bool exc_AT_CIFSR(char *list);
    bool set_AT_CIPMUX(uint8_t mode);
    bool set_AT_CIPSTART_single(char *type, char *addr, uint32_t port, uint32_t loca_port);
    bool exc_AT_CIPCLOSE_single(void);
    bool set_AT_CIPSTART_multiple(uint8_t mux_id, char *type, char *addr, uint32_t port, uint32_t loca_port);
    bool set_AT_CIPCLOSE_mulitple(uint8_t mux_id);
    bool set_AT_CIPSTO(uint32_t timeout);
    bool set_AT_CIPSERVER(uint8_t mode, uint32_t port = 333);
    bool set_AT_CIPSEND_single(const uint8_t *buffer, uint32_t len);
    bool set_AT_CIPSEND_multiple(uint8_t mux_id, const uint8_t *buffer, uint32_t len);
    bool set_AT_CIPMODE(uint8_t mode);//����͸��ģʽ0����͸����1��͸��ģʽֻ�йر�serverģʽ������Ϊ��·
    bool set_AT_exit_trans();//�Ƴ�͸��
    bool exc_AT_CIUPDATE(void);//���������̼�
    bool exc_AT_PING(const char *host, char *msg); //ping
    //SYS������ATָ��
    //������

private:
    ////����������ش���//////////////////////////////////////////////////////////
    uint8_t     net_data_buf[NET_DATA_BUFFER_SIZE];//���ڸ����λ�����
    RINGBUF     net_buf;//���ڽ����������ݵĻ��λ�����
    uint16_t    net_data_rx_cnt;
    uint16_t    net_data_len;
    uint16_t    net_data_id;


    char        rx_cmd_buf[RX_BUFFER_SIZE];//AT������ջ�����
    uint16_t    rx_cnt;//AT�����ַ������ռ�����
    void        clear_rx_cdm_buffer(void); //���AT�������
    void        get_char(void);

    //�ַ������ܼ���
    uint16_t    find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek);
    int         search_str(char *source, const char *target);
    uint16_t    get_str(char *source, const char *begin, uint16_t count1, const char *end, uint16_t count2, char *out);
    uint16_t    get_str(char *source, const char *begin, uint16_t count, uint16_t length, char *out);
    uint16_t    get_str(char *source, char *out, uint16_t length);
    //�����
    void        print_rx_buf();
    void        print_cmd(CMD_STATE_T &cmd);
    CMD_STATE_T wait_cmd(uint32_t wait_time);
    CMD_STATE_T wait_cmd(const char *spacial_target, uint32_t wait_time);
    bool        wait_wifi_mode(WIFI_MODE_T mode);


    //����
    uint32_t    last_time;
    CMD_STATE_T cmd_state;
    WIFI_MODE_T wifi_mode;



    //Ӳ����Դ
    Gpio    *rst;
    Uart   *uart; /* The UART to communicate with ESP8266 */

    friend void uart_interrupt_event(void);
    friend void net_data_state_process(char c);
    friend void net_data_state_process(char c);

};

extern ESP8266 wifi;

#endif /* #ifndef __ESP8266_H__ */

