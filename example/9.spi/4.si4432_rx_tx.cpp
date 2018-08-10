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
#include "button.h"
/**
	*	1	��������Ҫ����eDriveĿ¼�µ�si4432����
	*	2	��������ʾ��si4432����ģ���շ�
	*/

/* ���������������̷������� */
#define EXAMPLE_NAME	"si4432 example"
#define EXAMPLE_DATE	"2018-08-11"

Si4432	rf(&PA12,&PA11,&PB12,&spi2);

Led     led_net(&LED1,1);
Led     led_rx(&LED2,1);
Led		led_tx(&LED3,1);

Button btn(&BtnPin,1);
///////////////////////////////////
uint8_t     recv_buf[1024] = {0};
uint16_t    len = 0;
uint32_t    count = 0;

uint8_t rbuf[512];
uint8_t wbuf[512];


//���߷���ɼ�����
void RF_TxData(uint8_t protno)
{
	uint8_t TxBuf_RF[32];
	//������������ TxBuf_RF����
	TxBuf_RF[0] = 0x5A;			//��ͷ
	TxBuf_RF[1] = 0xA5;			//��ͷ
	TxBuf_RF[2] = 6;	 	//֡����
	TxBuf_RF[3] = protno;			//
	TxBuf_RF[4] = 0xAA;			//��β
	TxBuf_RF[4] = 0xBB;			//��β

	//FIFO����
	rf.write(TxBuf_RF,5+len);

	//�����ж�ʹ�ܼĴ���������Ϊֻ�е���Ч�����ݰ������ջ��߽��յ������ݰ�����CRCУ���������ж�
	rf.SpiWriteRegister(0x05, 0x03);
	rf.SpiWriteRegister(0x06, 0x00);
	//������Ϻ�����ģ�����״̬��
	rf.set_rx_mode();
}


void setup()
{
    ebox_init();
    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

	rf.begin(2);
	btn.begin();
	led_net.begin();
	led_rx.begin();
	led_tx.begin();

}
int main(void)
{
	int ret;
	uint16_t tx_cnt = 0;


	setup();

	//si4432ģ������
	rf.start();
	//si4432ģ������
	rf.set_config();
	//si4432����Ϊ����ģʽ
	rf.set_rx_mode();

	while(1)
	{
		btn.loop();
		if(btn.click())
		{
			tx_cnt++;
			RF_TxData(tx_cnt);
			led_tx.toggle();
		}


		//����ģ������ж�
		if(rf.read_irq() == 0)
		{
			//����ģ�鴦�ڿ���ģʽ�������յ������ݰ�����������������
			rf.set_idle_mode();
			uint8_t read_len = rf.read(rbuf);
			uart1.printf("\read:%d",read_len);
		}
	}
}



