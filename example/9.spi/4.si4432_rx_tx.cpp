/*
file   : *.cpp
author : shentq
version: V1.0
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.
*/

//STM32 RUN IN eBox
#include "ebox.h"
#include "bsp_ebox.h"

#include "si4432.h"
#include "led.h"
#include "EventGpio.h"
#include "EventManager.h"


/**
	*	1	此例程需要调用eDrive目录下的si4432驱动
	*	2	此例程演示了si4432无线模块收发
	*/

/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"si4432 example"
#define EXAMPLE_DATE	"2018-08-11"

Si4432	rf(&PA12, &PA11, &PB12, &spi2);

Led     led_net(&LED1, 1);
Led     led_rx(&LED2, 1);
Led		led_tx(&LED3, 1);

///////////////////////////////////
uint8_t     recv_buf[1024] = {0};
uint16_t    len = 0;
uint32_t    count = 0;

uint8_t rbuf[512];
uint8_t wbuf[512];
uint16_t tx_cnt = 0;



//无线发射采集数据
void RF_TxData(uint8_t protno)
{
    uint8_t TxBuf_RF[32];
    //发射数据整合 TxBuf_RF数组
    TxBuf_RF[0] = 0x5A;			//包头
    TxBuf_RF[1] = 0xA5;			//包头
    TxBuf_RF[2] = 6;	 	//帧长度
    TxBuf_RF[3] = protno;			//
    TxBuf_RF[4] = 0xAA;			//结尾
    TxBuf_RF[4] = 0xBB;			//结尾

    //FIFO发射
    rf.write(TxBuf_RF, 5 + len);

    //设置中断使能寄存器，设置为只有当有效的数据包被接收或者接收到的数据包数据CRC校验出错才来中断
    rf.SpiWriteRegister(0x05, 0x03);
    rf.SpiWriteRegister(0x06, 0x00);
    //发射完毕后设置模块接收状态；
    rf.set_rx_mode();
}
void click(Object *sender)
{
    UART.println("检测到按键单击");
    tx_cnt++;
    RF_TxData(tx_cnt);
    led_tx.toggle();
}

void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);

    rf.begin();
    
    btn.event_click = click;
    
    btn.begin();
    manager.add(&btn);

    led_net.begin();
    led_rx.begin();
    led_tx.begin();

}
int main(void)
{
    int ret;


    setup();

    //si4432模块启动
    rf.start();
    //si4432模块配置
    rf.set_config();
    //si4432设置为接收模式
    rf.set_rx_mode();

    while(1)
    {
        manager.loop();

        //无线模块接收中断
        if(rf.read_irq() == 0)
        {
            //设置模块处于空闲模式，处理收到的数据包，不继续接收数据
            rf.set_idle_mode();
            uint8_t read_len = rf.read(rbuf);
            uart1.printf("\read:%d", read_len);
        }
    }
}



