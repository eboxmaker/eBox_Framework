/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox


#include "ebox.h"
#include "w5500.h"
#include "ebox_slave_spi.h"

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
    uart1.printf("【%d】",ch);
    
    switch(fsm_status)
    {
        case FSM_NULL:
            uart1.printf("NULL: to %d\r\n",ch);
            if(is_valid_cmd(ch) && cmd_prepare(ch))
            {
                slave_spi1.write(status);
                cmd = ch;
                if ((CMD_GET_STATUS == ch))   // || ((CMD_7688_WRITE_TO_STM32 == ch) ) || ((CMD_7688_READ_FROM_STM32 == ch))
                    fsm_status_next = FSM_SEND_END_RESP;
				else
                {
					fsm_status_next = FSM_SEND_RESP;
                    uart1.printf("下一个255走向SEND RESP\r\n");
                }
            }
            else
            {
                slave_spi1.write(0);
                uart1.printf("错误的命令\r\n");

            }
            break;
        case FSM_SEND_RESP:
			/* write spi data reg first if change to write status */
			if (CMD_SET_BLOCK_LEN == cmd) {
				slave_spi1.write(0xcd);
				fsm_status_next = FSM_SET_BLOCK_LEN;
                uart1.printf("OK:CMD_SET_BLOCK_LEN %d\r\n",ch);
			} else if (CMD_7688_WRITE_TO_STM32 == cmd) {
				slave_spi1.disable_rx_irq();
                slave_spi1.enable_rx_dma();
                uart1.printf("OK:ENABLE RX DMA\r\n",ch);

				//send_ch(0);
				fsm_status_next = FSM_7688_WRITE_TO_STM32;
			} else if (CMD_7688_READ_FROM_STM32 == cmd) {
				fsm_status_next = FSM_7688_READ_FROM_STM32;
			}
			break;
        case FSM_SET_BLOCK_LEN:
            uart1.printf("OK:set block len\r\n");
			if (!is_valid_len(ch)) {
                uart1.printf("bad len:%d\r\n",ch);
            }
            if(slave_spi1.xlen == ch)
            {
            }
                uart1.printf("OK:xlen = len = %d\r\n",slave_spi1.xlen);
            if(slave_spi1.xfet != NULL)
            {
                uart1.printf("OK:非空：释放\r\n");
            }
            else
                uart1.printf("OK:first： \r\n");

            slave_spi1.xfet = (uint8_t *)ebox_malloc(ch);
            if(slave_spi1.xfet == NULL)
            {
                uart1.printf("malloc failed \r\n");
            }
            slave_spi1.xlen = ch;
            slave_spi1.write(status);
            fsm_status_next = FSM_SEND_END_RESP;

            break;
        case FSM_7688_WRITE_TO_STM32:
            slave_spi1.disable_rx_dma();
            slave_spi1.enable_rx_int();
            break;
		case FSM_7688_READ_FROM_STM32:
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
//    slave_spi1.disable_rx_irq();
//    delay_ms(1);
     ch = SPI1->DR;;
    slave_spi1.write_buf.write(ch);
//            slave_spi1.dma_buf = (uint8_t *)ebox_malloc(10);
//    memset(slave_spi1.dma_buf,0x31,3);
//    slave_spi1.enable_dma_send(slave_spi1.dma_buf,3);
//    slave_spi1.enable_dma_tc_irq();
//    uart1.println(ch);
    PA2.set();
}
void tc()
{
//    slave_spi1.disable_dma_send();
//    slave_spi1.disable_dma_tc_irq();
//    while ((SPI1->SR & SPI_I2S_FLAG_RXNE) == RESET);
//    while ((SPI1->SR & SPI_I2S_FLAG_TXE) == RESET);
//    while((SPI1->SR & SPI_I2S_FLAG_BSY) == SET);
////    delay_ms(1);
////    uart1.printf("DMA %d\r\n",updata);
//    PA2.reset();
//    slave_spi1.enable_rx_irq();

}
void setup()
{
    ebox_init();
    uart1.begin(512000);
    uart1.printf("\r\nuart1 115200 ok!");
    slave_spi1.begin();
    slave_spi1.attach(test,SpiRxIrq);
    slave_spi1.attach(tc,SpiTcIrq);
    PA2.mode(OUTPUT_PP);


}
int main(void)
{
    setup();

    while(1)
    {
        if(slave_spi1.write_buf.available())
        {
            uint8_t ch =slave_spi1.write_buf.read();
            process(ch);
        }

//        delay_ms(1000);
    }


}




