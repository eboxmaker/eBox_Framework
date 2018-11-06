#include "spibridge.h"



#if 0
#define SPI_DEBUG(...)uart1.printf(__VA_ARGS__)
#else
#define  SPI_DEBUG(...)
#endif

uint8_t updata = 0;
uint8_t ch;
uint8_t state = 0;
uint8_t buf[100];
uint8_t *ptr;


#define CMD_NULL                    0
#define CMD_GET_STATUS              1
#define CMD_7688_READ_FROM_STM32    2
#define CMD_7688_WRITE_TO_STM32     3
#define CMD_SET_BLOCK_LEN           4

/* status of FSM */
#define FSM_7688_READ_FROM_STM32         (2U)
#define FSM_7688_WRITE_TO_STM32          (3U)
#define FSM_SET_BLOCK_LEN                (4U)
#define FSM_SEND_RESP                    (5U)
#define FSM_SEND_END_RESP                (6U)
#define FSM_RECV_END_RESP                (7U)
#define FSM_NULL                         (0U)

#define SPI_BRIDGE_LEN_8_BYTES                      (8U)
#define SPI_BRIDGE_LEN_16_BYTES                     (16U)
#define SPI_BRIDGE_LEN_32_BYTES                     (32U)

uint8_t cmd;
uint8_t status = 0XAA;
uint8_t fsm_status;
uint8_t fsm_status_next;

static inline bool is_valid_cmd(uint8_t cmd)
{
    return ((CMD_GET_STATUS == cmd)
            || (CMD_7688_READ_FROM_STM32 == cmd)
            || (CMD_7688_WRITE_TO_STM32 == cmd)
            || (CMD_SET_BLOCK_LEN == cmd));
}
bool cmd_prepare(uint8_t cmd)
{
    if(cmd == CMD_7688_READ_FROM_STM32)
    {
        slave_spi1.set_tx_dma();
    }
    else if(CMD_7688_WRITE_TO_STM32)
    {
        slave_spi1.set_rx_dma();
    }
    else if(cmd == CMD_SET_BLOCK_LEN)
    {

    }
    else if(cmd == CMD_GET_STATUS)
    {

    }
    else
    {
        return false;
    }
    return true;

}
static inline bool is_valid_len(uint8_t len)
{
    return ((SPI_BRIDGE_LEN_8_BYTES == len)
            || (SPI_BRIDGE_LEN_16_BYTES == len)
            || (SPI_BRIDGE_LEN_32_BYTES == len));
}

void process(uint8_t ch)
{
    fsm_status = fsm_status_next;
    SPI_DEBUG("¡¾%d¡¿", ch);

    switch(fsm_status)
    {
    case FSM_NULL:
        SPI_DEBUG("NULL: to %d\r\n", ch);
        if(is_valid_cmd(ch) && cmd_prepare(ch))
        {
            slave_spi1.write(status);
            cmd = ch;
            if ((CMD_GET_STATUS == ch))   // || ((CMD_7688_WRITE_TO_STM32 == ch) ) || ((CMD_7688_READ_FROM_STM32 == ch))
                fsm_status_next = FSM_SEND_END_RESP;
            else
            {
                fsm_status_next = FSM_SEND_RESP;
            }
        }
        else
        {
            slave_spi1.write(0);

        }
        break;
    case FSM_SEND_RESP:
        /* write spi data reg first if change to write status */
        if (CMD_SET_BLOCK_LEN == cmd)
        {
            slave_spi1.write(0xcd);
            fsm_status_next = FSM_SET_BLOCK_LEN;
            SPI_DEBUG("OK:CMD_SET_BLOCK_LEN %d\r\n", ch);
        }
        else if (CMD_7688_WRITE_TO_STM32 == cmd)
        {
            slave_spi1.disable_rx_int();
            slave_spi1.enable_rx_dma();
            SPI_DEBUG("OK:ENABLE RX DMA\r\n", ch);

            //send_ch(0);
            fsm_status_next = FSM_7688_WRITE_TO_STM32;
        }
        else if (CMD_7688_READ_FROM_STM32 == cmd)
        {
            slave_spi1.disable_rx_int();
            slave_spi1.enable_tx_dma();
            SPI_DEBUG("OK:ENABLE tX DMA\r\n", ch);

            fsm_status_next = FSM_7688_READ_FROM_STM32;
        }
        break;
    case FSM_SET_BLOCK_LEN:
        SPI_DEBUG("OK:set block len\r\n");
        if (!is_valid_len(ch))
        {
            SPI_DEBUG("bad len:%d\r\n", ch);
        }
        if(slave_spi1.xlen == ch)
        {
        }
        SPI_DEBUG("OK:xlen = len = %d\r\n", slave_spi1.xlen);
        if(slave_spi1.xfet != NULL)
        {
            SPI_DEBUG("OK:·Ç¿Õ£ºÊÍ·Å\r\n");
            ebox_free(slave_spi1.xfet);

        }
        else
            SPI_DEBUG("OK:first£º \r\n");

        slave_spi1.xfet = (uint8_t *)ebox_malloc(ch);
        if(slave_spi1.xfet == NULL)
        {
            SPI_DEBUG("malloc failed \r\n");
        }
        slave_spi1.xlen = ch;
        slave_spi1.write(status);
        fsm_status_next = FSM_SEND_END_RESP;

        break;
    case FSM_7688_WRITE_TO_STM32:
        slave_spi1.disable_rx_dma();
        slave_spi1.enable_rx_int();
        uart1.write(slave_spi1.xfet, 32);
        uart1.println();
        fsm_status_next = FSM_NULL;

        break;
    case FSM_7688_READ_FROM_STM32:
        slave_spi1.disable_tx_dma();
        slave_spi1.enable_rx_int();
        fsm_status_next = FSM_NULL;

        break;
    case FSM_RECV_END_RESP:
    case FSM_SEND_END_RESP:
    default:
        cmd = CMD_NULL;
        fsm_status_next = FSM_NULL;
        break;
    }

}
void test()
{

    slave_spi1.cmd_buf.write(SPI1->DR);

    PA2.set();
}
void recv_over_event()
{
    slave_spi1.cmd_buf.write(0);

    //uart1.printf("111111111111111111111111111111111111111111111\r\n");
}
void send_over_event()
{
    slave_spi1.cmd_buf.write(0);
    //  uart1.printf("2222222222222222222222222222222222222222222222\r\n");
}
void begin()
{
    slave_spi1.begin();
    slave_spi1.attach(test, SpiItRx);
    slave_spi1.attach(recv_over_event, SpiItRc);
    slave_spi1.attach(send_over_event, SpiItTc);

}
void loop()
{
    if(slave_spi1.cmd_buf.available())
    {
        uint8_t ch = slave_spi1.cmd_buf.read();
        process(ch);
    }
}