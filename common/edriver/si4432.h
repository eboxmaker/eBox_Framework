/**
  ******************************************************************************
  * @file    si4432.h
  * @author  songsj
  * @version V1.0
  * @date    2016/10/8
  * @brief
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of songsj. This specification is
  * preliminary and is subject to change at any time without notice. songsj assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2016 songsj. All Rights Reserved.</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef __SI4432_H
#define __SI4432_H
#include "ebox.h"

//Si4432�жϹܽ�	 P2.13
//#define  Si4432_NIRQ_ST()  	    FIO2_ST(13)

////��������

////���߷���ɼ�����
//void RF_TxData(uint8 protno,uint8 len,char *str);

//void rx_data(void);

////ȫ�ֳ���
//#define SPI_CLOCK_DELAY   2  //SPIʱ���ӳ�

class Si4432
{
public:
	Si4432(Gpio *irq, Gpio *ce, Gpio *cs, Spi *spi);
	void begin(uint8_t dev_num);
	void start(void);
	void set_config(void);
	uint8_t read(uint8_t *rxbuf);
	void write(uint8_t *txdata, uint8_t len_tx);
	void set_rx_mode(void);		//ģʽ����:����
	void set_tx_mode(void);		//ģʽ����:����
	void set_idle_mode(void);	//ģʽ����:����
	uint8_t read_rssi(void);	//��ȡ�����ź�ǿ��
	uint8_t read_irq(void);		//��ȡirq�ܽ�״̬
	uint8_t SpiWriteRegister (uint8_t reg, uint8_t value); //SPIд����ģ��Ĵ���
	uint8_t SpiReadRegister (uint8_t reg);	//SPI������ģ��Ĵ���

private:
	uint8_t             spi_flash_buf[256];
	SPI_CONFIG_TYPE     spi_dev_si4432;
	Gpio				*irq;
	Gpio				*ce;
	Gpio				*cs;
	Spi					*spi;

	
	void reset(void);
#define TxGPIOSetting()   SpiWriteRegister(0x0E, 0x01)
#define RxGPIOSetting()   SpiWriteRegister(0x0E, 0x02)
#define IdleGPIOSetting() SpiWriteRegister(0x0E, 0x00)
};

#endif
