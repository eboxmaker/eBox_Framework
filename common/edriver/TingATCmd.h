/**
  ******************************************************************************
  * @file    tingATCmd.h
  * @author  shentq
  * @version V0.3
  * @date    2017/06/25
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ebox.h"

#define CMD_BUFFER_SIZE 100
#define DATA_BUFFER_SIZE 255
typedef struct sLoRaSettings
{
    uint32_t RFFrequency;
    int8_t Power;
    uint8_t SignalBw;                   // LORA [0: 7.8 kHz, 1: 10.4 kHz, 2: 15.6 kHz, 3: 20.8 kHz, 4: 31.2 kHz,
                                        // 5: 41.6 kHz, 6: 62.5 kHz, 7: 125 kHz, 8: 250 kHz, 9: 500 kHz, other: Reserved]  
    uint8_t SpreadingFactor;            // LORA [6: 64, 7: 128, 8: 256, 9: 512, 10: 1024, 11: 2048, 12: 4096  chips]
    uint8_t ErrorCoding;                // LORA [1: 4/5, 2: 4/6, 3: 4/7, 4: 4/8]
    bool CrcOn;                         // [0: OFF, 1: ON]
    bool ImplicitHeaderOn;              // [0: OFF, 1: ON]
    bool RxSingleOn;                    // [0: Continuous, 1 Single]
    bool FreqHopOn;                     // [0: OFF, 1: ON]
    uint8_t HopPeriod;                  // Hops every frequency hopping period symbols
    //uint32_t TxPacketTimeout;           //????????
    uint32_t RxPacketTimeout;           //????????
    uint8_t PayloadLength;              //??????:1-255
    uint16_t PreambleLength;            //?????4-1024
}tLoRaSettings;

typedef enum
{
    MODE_IDLE = 0,
    MODE_AT ,
    MODE_AT_END ,
    MODE_DATA ,
    MODE_AT_TIMEOUT,
    MODE_AT_TOO_LONG,
} CMD_MODE_T;
typedef enum
{
    ERR_OK = 0,
    ERR_CMD,
    ERR_CPU_BUSY,
    ERR_RF_BUSY ,
    ERR_SYMBLE,
    ERR_PARA,
    ERR_TIMEOUT,
    ERR_OTHER,
} CMD_ERR_T;
typedef enum
{
    NEED_DOT1 = 0,
    NEED_SADDR_DATA,
    NEED_DOT2 ,
    NEED_LEN_DATA,
    NEED_DOT3 ,
    NEED_DATA ,
    STATE_ERROR ,
    STATE_END ,
} DATA_STATE_T;
typedef enum
{
    LR_IDLE = 0,
    LR_CAD ,
    LR_RX ,
    LR_SENDING,
}RF_STATE_T;
class Ting
{
    public:
        tLoRaSettings LoRaSettings;
        bool      begin(Gpio *rst,Gpio *wakeup, Uart *uart, uint32_t baud);
        void      hard_reset();
        CMD_ERR_T soft_reset();
        CMD_ERR_T test();
        CMD_ERR_T config(sLoRaSettings *settings);
        CMD_ERR_T get_version(char *msg,uint8_t *len);
        CMD_ERR_T idle();
        CMD_ERR_T sleep();
        CMD_ERR_T wakeup();
        CMD_ERR_T rx();
        CMD_ERR_T rssi(char *msg,uint8_t *len);
        CMD_ERR_T set_addr(uint16_t addr);
        CMD_ERR_T get_addr(uint16_t *addr);
        CMD_ERR_T set_dest(uint16_t addr);
        CMD_ERR_T get_dest(uint16_t *addr);
        CMD_ERR_T save();    
        CMD_ERR_T send(uint8_t *ptr,uint8_t len);
        uint8_t   available();
        uint8_t   read(uint8_t *buf,uint8_t len);
        uint8_t   is_rx_timeout();

        CMD_ERR_T set_pb0();
        CMD_ERR_T clear_pb0();
        CMD_ERR_T read_pb0();
        CMD_ERR_T set_pd0();
        CMD_ERR_T clear_pd0();
        CMD_ERR_T read_pd0();
        CMD_ERR_T pwm1(uint8_t prescaler,uint16_t period,uint16_t pulse);
        CMD_ERR_T pwm2(uint8_t prescaler,uint16_t period,uint16_t pulse);
    private:    
        uint8_t rx_timeoutflag;
        bool rx_signal_on;
    
        void            rx_evend();//接收中断事件处理
        DATA_STATE_T    data_process(char c);//接收中断事件处理->之接收到的数据
        void            timeout_process();//接收中断事件处理->之接收到的数据
    
        void         clear_rx_cdm_buffer(void); //清空AT命令缓冲区
        CMD_MODE_T   _wait_ack(uint16_t timeout);//等待命令回复
        CMD_ERR_T    update_cmd_err();//解析命令错误意义
        void         debug_cmd_err(const char *str);//调试输出命令错误信息
    
        int search_str(char *source, const char *target);
    private:
        //接收数据相关变量
        DATA_STATE_T    data_state;
        uint8_t         data_count;
        uint8_t         data_buf[DATA_BUFFER_SIZE];
        RINGBUF         pDataBuf;//用于接收网络数据的环形缓冲区
        char            source_addr_buf[5];
        char            len_buf[3];
        uint16_t        source_addr;
        uint8_t         data_len;
        //命令相关变量
        char            rx_cmd_buf[CMD_BUFFER_SIZE];
        uint32_t        last_rx_event_time;
        uint8_t         rx_count;
        CMD_ERR_T       cmd_err;
        //硬件接口
        Gpio    *rst;
        Gpio    *_wakeup;
        Uart    *uart; /* The UART to communicate with ESP8266 */

};
