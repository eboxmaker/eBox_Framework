/*
file   : softspi.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __SOFTSPI_H
#define __SOFTSPI_H

#include "common.h"

/*
	1.支持任何IO引脚的定义
	2.该spi功能强大，总线支持同时挂载不同MODE ,SPEED,bitOder的设备
	3.由于支持softspi，统一化宏参数，将以下的宏定义在ebox.h
	#define SPI_MODE0 0x02
	#define SPI_MODE1 0x00
	#define SPI_MODE2 0x03
	#define SPI_MODE3 0x01

	#define SPI_CLOCK_DIV2     SPI_BaudRatePrescaler_2  
	#define SPI_CLOCK_DIV4     SPI_BaudRatePrescaler_4  
	#define SPI_CLOCK_DIV8     SPI_BaudRatePrescaler_8  
	#define SPI_CLOCK_DIV16    SPI_BaudRatePrescaler_16 
	#define SPI_CLOCK_DIV32    SPI_BaudRatePrescaler_32 
	#define SPI_CLOCK_DIV64    SPI_BaudRatePrescaler_64 
	#define SPI_CLOCK_DIV128   SPI_BaudRatePrescaler_128
	#define SPI_CLOCK_DIV256   SPI_BaudRatePrescaler_256

	#define SPI_BITODER_MSB		SPI_FirstBit_MSB                
	#define SPI_BITODER_LSB		SPI_FirstBit_LSB    
	typedef struct
	{
		uint8_t devNum;//
		uint8_t mode;
		uint16_t prescaler;
		uint16_t bitOrder;
	}SPICONFIG;
	4.每一个spi设备应有一个自己的SPICONFIG的配置，以支持该设备的的读写，
		在读写前需要检测当前spi配置是否的devNum，如果是就跳过config，如果不是
		则调用config(SPICONFIG* spiConfig);
		
	注意：1.该类的SPI_CLOCK_DIV是由delay_us延时函数控制。略有不准，比硬件SPI会慢很多
				2.speed设置只能为SPI_CLOCK_DIVx。如果不是此值，则会将SPI_CLOCK_DIV的值直接传递给delay_us.即delay_us(SPICONFIG->prescaler);
				3.初期调试I2C设备建议使用SPI_CLOCK_DIV256。

*/


//默认配置 空，只依靠结构体SPICONFIG来初始化



class SOFTSPI
{
	public:
		SOFTSPI(GPIO* sckPin,GPIO* misoPin,GPIO* mosiPin);
	
		void 		begin(SPICONFIG* spiConfig);
	  void 		config(SPICONFIG* spiConfig);
		uint8_t readConfig(void);
	
		uint8_t transfer(uint8_t data);

	private:
		GPIO* 	_sckPin;
		GPIO*		_mosiPin;
		GPIO*		_misoPin;
		
	
		uint8_t _mode;	
		uint8_t _bitOrder;
		uint8_t _spidelay;
	
		uint8_t currentDevNum;


};
extern SOFTSPI sspi;

#endif

