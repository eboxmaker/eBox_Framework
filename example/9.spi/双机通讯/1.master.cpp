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


SpiConfig_t config;
uint8_t txbuf[32];
uint8_t rxbuf[32];
uint8_t len;

void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.printf("\r\nuart1 115200 ok!");

    config.dev_num = 0;
    config.bit_order = SPI_FirstBit_MSB;
    config.mode = SPI_MODE0;
    config.prescaler = SPI_BaudRatePrescaler_256;

    spi1.begin(&config);
    PA4.mode(OUTPUT_PP);
    PA4.set();

    for(int i = 0; i < 32; i++)
    {
        txbuf[i] = 0x30 + i;
    }
}
int main(void)
{
    setup();

    while(1)
    {

        //get status
        PA4.reset();
        spi1.write(CMD_GET_STATUS);
        delay_ms(1);
        uart1.printf("CMD status:0X%X\r\n", spi1.transfer(0XFF));
        delay_ms(1);
        PA4.set();

        //set len 32
        PA4.reset();
        spi1.write(CMD_SET_BLOCK_LEN);
        delay_ms(1);
        uart1.printf("CMD SET BLOCK:0X%X\r\n", spi1.read());
        delay_ms(1);
        spi1.write(32);
        delay_ms(1);
        uart1.printf("DATA LEN:0X%X\r\n", spi1.read());
        delay_ms(1);
        PA4.set();

        //write to slave
        PA4.reset();
        spi1.write(CMD_7688_WRITE_TO_STM32);
        delay_ms(1);
        uart1.printf("CMD_7688_WRITE_TO_STM32 :0X%X\r\n", spi1.read());
        delay_ms(1);
        spi1.write(txbuf, 32);
        delay_ms(1);
        uart1.printf("write 32 bytes\r\n");
        delay_ms(1);
        PA4.set();


        //get status
        PA4.reset();
        spi1.write(CMD_GET_STATUS);
        delay_ms(1);
        uart1.printf("CMD status:0X%X\r\n", spi1.transfer(0XFF));
        delay_ms(1);
        PA4.set();



        //read form slave
        PA4.reset();
        spi1.write(CMD_7688_READ_FROM_STM32);
        delay_ms(1);
        uart1.printf("CMD_7688_READ_FROM_STM32 :0X%X\r\n", spi1.read());
        delay_ms(1);
        spi1.read(rxbuf, 32);
        uart1.write(rxbuf, 32);
        delay_ms(1);
        delay_ms(1);
        PA4.set();







        uart1.println();

        delay_ms(1000);
    }


}




