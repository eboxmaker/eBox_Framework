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

//Si4432中断管脚	 P2.13
//#define  Si4432_NIRQ_ST()  	    FIO2_ST(13)

////函数声明

////无线发射采集数据
//void RF_TxData(uint8 protno,uint8 len,char *str);

//void rx_data(void);

////全局常量
//#define SPI_CLOCK_DELAY   2  //SPI时钟延迟

class Si4432
{
public:
	Si4432(Gpio *irq, Gpio *ce, Gpio *cs, Spi *spi);
	void begin(uint8_t dev_num);
	void start(void);
	void set_config(void);
	uint8_t read(uint8_t *rxbuf);
	void write(uint8_t *txdata, uint8_t len_tx);
	void set_rx_mode(void);		//模式设置:接收
	void set_tx_mode(void);		//模式设置:发送
	void set_idle_mode(void);	//模式设置:空闲
	uint8_t read_rssi(void);	//读取接收信号强度
	uint8_t read_irq(void);		//读取irq管脚状态
	uint8_t SpiWriteRegister (uint8_t reg, uint8_t value); //SPI写无线模块寄存器
	uint8_t SpiReadRegister (uint8_t reg);	//SPI读无线模块寄存器

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
