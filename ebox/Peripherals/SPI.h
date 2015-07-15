/*
file   : spi.h
author : shentq
version: V1.0
date   : 2015/7/5

Copyright (c) 2015, eBox by shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef __SPI_H
#define __SPI_H

#include "common.h"
//spi相关公共宏和声明//////////////////////////////////
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
	uint8_t devNum;
	uint8_t mode;
	uint16_t prescaler;
	uint16_t bitOrder;
}SPICONFIG;


/*
	1.目前只测试了SPI1，理论支持SPI2和spi3，望网友测试
	2.该spi功能强大，总线支持同时挂载不同MODE ,SPEED,bitOder的设备
	3.每一个spi设备应有一个自己的SPICONFIG的配置，以支持该设备的的读写，
		在读写前需要检测当前spi配置是否的devNum，如果是就跳过config，如果不是
		则调用config(SPICONFIG* spiConfig);
*/
//默认配置 空，只依靠结构体SPICONFIG来初始化

class	SPIClASS 
{
	public:
		SPIClASS(SPI_TypeDef *SPIx,GPIO* sckPin,GPIO* misoPin,GPIO* mosiPin);
	
		void begin (SPICONFIG* spiConfig);
		void config(SPICONFIG* spiConfig);
		uint8_t readConfig(void);
		
		uint8_t transfer(uint8_t data);
		void transfer(uint8_t *data,uint16_t dataln);
		void transfer(uint8_t dummyByte,uint8_t *rcvdata,uint16_t dataln);
		
	private:
		static uint8_t currentDevNum;
		SPI_TypeDef *spi;

};
/*
	注意：1.该类的SPI_CLOCK_DIV是由delay_us延时函数控制。略有不准，比硬件SPI会慢很多
				2.speed设置只能为SPI_CLOCK_DIVx。如果不是此值，则会将SPI_CLOCK_DIV的值直接传递给delay_us.即delay_us(SPICONFIG->prescaler);
				3.初期调试I2C设备建议使用SPI_CLOCK_DIV256。
*/
class SOFTSPI
{
	public:
		SOFTSPI(GPIO* SCKPin,GPIO* MISOPin,GPIO* MOSIPin);
	
		void 		begin(SPICONFIG* spiConfig);
	  void 		config(SPICONFIG* spiConfig);
		uint8_t readConfig(void);
	
		uint8_t transfer(uint8_t data);

	private:
		GPIO* 	sckPin;
		GPIO*		mosiPin;
		GPIO*		misoPin;
			
		uint8_t mode;	
		uint8_t bitOrder;
		uint8_t spidelay;
	
		static uint8_t currentDevNum;
};

#endif
