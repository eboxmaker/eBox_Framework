#ifndef __USR_GM3_H
#define __USR_GM3_H
#include "ebox.h"

#define NET_BUF_SIZE 256
typedef enum 
{
    INIT,
    CMD,
    SMS,
    NET,
    HTTPD,
}GM3Mode_t;
typedef enum
{
    IDLE,
    SENDED,
    WAITTING,
    RECEIVING,
    RECEIVED
}UartState_t;
typedef enum
{
    H_COM,
    H_NET,
}HEARTTP_t;
typedef enum
{
    DISCONNECTED = 0,
    CONNECTED = 1,
}LinkState_t;

class UsrGm3
{
    public:
        RINGBUF pNetBuf;//用于接收网络数据的环形缓冲区

    public:
        UsrGm3(Uart *uart,Gpio *rst,Gpio *link,Gpio *linka,Gpio *linkb)
        {
            gm3_mode = INIT;
            this->uart = uart;
            this->rst = rst;
            this->link = link;
            this->linka = linka;
            this->linkb = linkb;
        }
        void begin(uint32_t baud_rate);
        void hard_reset();
        void soft_reset();
        void at_cmd_go();
        uint16_t at_cmd_send(char *cmd,char *result);
        uint16_t at_cmd_send(const char *cmd,char *result);
        LinkState_t link_state();
        LinkState_t linka_state();
        LinkState_t linkb_state();
        
        void send(char *buf,uint16_t len);



        //管理指令
        void h();
        void z();
        void q_e();
        void entm();
        void q_wkmod();
        void q_calen();
        void q_naten();
        void q_uaten();
        void q_cmdpw();
        void q_cachen();
        void q_stmsg();
        uint16_t q_rstim();
        void q_sleep();
        uint16_t q_sleeptim();   


        void set_e(uint8_t enable);
        void set_wkmod(GM3Mode_t mode);
        void set_calen(uint8_t enable);
        void set_naten(uint8_t enable);
        void set_uaten(uint8_t enable);
        void set_cmdpw(char *pwd);
        void set_cachen(uint8_t enable);
        void set_stmsg(char *msg);
        void set_rstim(uint16_t time);
        void set_sleep(uint8_t enable);
        void set_sleeptim(uint16_t time);          
        //配置参数指令
        void save();
        void reld();
        void clear();
        void cfgtf();
        
        
        //信息查询指令
        void q_version();
        void q_sn();
        void q_iccid();
        void q_imei();
        void q_cnum();
        
        //串口参数指令
        void q_uart();
        void q_rfcen();
        void set_uart(uint32_t baud,uint8_t parity,uint8_t data,uint8_t bits,uint8_t flow_ctr);
        void set_rfcen(uint8_t enable);        
        
        //网络指令
        void q_apn();
        void q_socka();
        void q_sockb();
        void q_sockaen();
        void q_sockben();
        void q_sockaals();
        void q_sockbsl();
        void q_sockalk();
        void q_sockblk();
        void q_sockiden();
        void set_apn();
        void set_socka(char* type, char *host,char *port);
        void set_sockb(char* type, char *host,char *port);
        void set_sockaen(uint8_t enable);
        void set_sockben(uint8_t enable);
        void set_sockaals();
        void set_sockbsl();
        void set_sockiden();  
        
       
        
        //心跳包指令
        void set_hearten(uint8_t enable);
        void set_heartdt(char *data);
        void set_hearttp(HEARTTP_t type);
        void set_hearttm(uint8_t time);
        uint8_t q_hearten();
        void q_heartdt(char *p);
        HEARTTP_t q_hearttp();
        uint8_t q_hearttm();
        
        //HTTPD指令
        
        //短信息指令
        
        //透传云功能
        
        //其他功能
        
        

    
    private:
        void rx_event();
        uint16_t wait(uint16_t timeout);
        uint16_t wait(const char *s,uint16_t timeout);
        uint16_t wait_end( uint16_t timeout);

        uint8_t at_3plus();
        void clear_cmd_buf();
    
    private:
        Uart *uart;
        Gpio *rst,*linka,*linkb,*link;
        GM3Mode_t gm3_mode;
        GM3Mode_t gm3_new_mode;
        UartState_t u_state;
        char cmd_buf[256];
        uint8_t net_buf[NET_BUF_SIZE];
        uint64_t start_time;
        uint64_t last_rx_event_time;


};

#endif
