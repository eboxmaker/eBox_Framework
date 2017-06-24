
#include "ebox.h"
#include "ringbuf.h"

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
    WAITING = 0,
    RECEIVING = 1,
    RECEIVED  = 2,
    TIMEOUT = 3
} CMD_STATE_T;

typedef enum
{
    ERR_OK = 0,
    ERR_CMD,
    ERR_CPU_BUSY,
    ERR_RF_BUSY ,
    ERR_SYMBLE,
    ERR_PARA,
    ERR_NONE,
    ERR_NO_ACK,
} CMD_ERR_T;
typedef enum
{
    NEED_PLUS = 0,
    NEED_L ,
    NEED_R ,
    NEED_DOT1 ,
    NEED_SADDR_DATA,
    NEED_DOT2 ,
    NEED_LEN_DATA,
    NEED_DOT3 ,
    NEED_DATA ,
    STATE_ERROR ,
} DATA_STATE_T;
class Ting
{
    public:
        tLoRaSettings LoRaSettings;
        bool begin(Gpio *rst,Gpio *wakeup, Uart *uart, uint32_t baud);
        void hard_reset();
        CMD_ERR_T soft_rst();

        void rx_evend();
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
        CMD_ERR_T send(uint8_t *ptr,uint8_t len);
        uint8_t available();
        uint8_t read(uint8_t *buf);

        CMD_ERR_T set_pb0();
        CMD_ERR_T clear_pb0();
        CMD_ERR_T read_pb0();
        CMD_ERR_T set_pd0();
        CMD_ERR_T clear_pd0();
        CMD_ERR_T read_pd0();
        CMD_ERR_T pwm1(uint8_t prescaler,uint16_t period,uint16_t pulse);
        CMD_ERR_T pwm2(uint8_t prescaler,uint16_t period,uint16_t pulse);

        CMD_ERR_T wait_ack(char *target,uint16_t timeout);
        CMD_ERR_T wait_ack(uint16_t timeout);
        CMD_STATE_T wait_rx_timeout();
        CMD_ERR_T update_cmd_err();
        void debug_cmd_err(const char *str);
        uint16_t find_str(uint8_t *s_str, uint8_t *p_str, uint16_t count, uint16_t &seek);
        int search_str(char *source, const char *target);
        
        CMD_STATE_T cmd_state;
        CMD_ERR_T   cmd_err;
        uint32_t last_rx_event_time;
        uint8_t rx_count;
        void    clear_rx_cdm_buffer(void); //清空AT命令缓冲区
        void    data_process(char c);
    private:
        uint8_t     data_buf[DATA_BUFFER_SIZE];
        char        source_addr_buf[5];
        char        len_buf[3];
        uint16_t    source_addr;
        uint8_t     data_len;

        RINGBUF     pDataBuf;//用于接收网络数据的环形缓冲区
        char        rx_cmd_buf[CMD_BUFFER_SIZE];

        Gpio    *rst;
        Gpio    *_wakeup;
        Uart    *uart; /* The UART to communicate with ESP8266 */

};