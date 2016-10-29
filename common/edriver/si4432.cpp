/**
  ******************************************************************************
  * @file    si4432.cpp
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


/* Includes ------------------------------------------------------------------*/
#include "si4432.h"

uint8_t  ItStatus1, ItStatus2;

Si4432::Si4432(Gpio *irq, Gpio *ce, Gpio *cs, Spi *spi)
{
    this->irq = irq;
    this->ce = ce;
    this->cs = cs;
    this->spi = spi;
}

/****************************************************************************
** 函数名称：begin
** 函数功能：Si4432初始化
** 入口参数：无
** 出口参数：无
****************************************************************************/
void Si4432::begin(uint8_t dev_num)
{
    spi_dev_si4432.dev_num = dev_num;
    spi_dev_si4432.mode = SPI_MODE0;
    spi_dev_si4432.prescaler = SPI_CLOCK_DIV4;
    spi_dev_si4432.bit_order = SPI_BITODER_MSB;
    spi->begin(&spi_dev_si4432);
    cs->mode(OUTPUT_PP);
    cs->set();
	ce->mode(OUTPUT_PP);
	ce->set();
	irq->mode(INPUT_PU);
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  + 函数名:	void SpiWriteRegister(uint8_t reg, uint8_t value)
  + 输入:	uint8_t reg - 寄存器地址
  			uint8_t value - 写入值
  + NOTES:  SPI写入SI4432寄存器
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
uint8_t Si4432::SpiWriteRegister (uint8_t reg, uint8_t value)
{
    spi->take_spi_right(&spi_dev_si4432);
    cs->reset();
    spi->write(reg | 0x80);
    spi->write(value);
    cs->set();
    spi->release_spi_right();
    return 1;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  + 函数名: uint8_t SpiReadRegister(uint8_t reg)
  + 输入:	uint8_t reg - 寄存器地址
  + 返回:   SPI1DAT - 内容
  + NOTES:  SPI读取SI4432寄存器
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
uint8_t Si4432::SpiReadRegister (uint8_t reg)
{
	uint8_t ret;
    spi->take_spi_right(&spi_dev_si4432);
    cs->reset();
    spi->write(reg);
    ret = spi->read();
    cs->set();
    spi->release_spi_right();
    return ret;
}

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  + 函数名: void set_config()
  + NOTES:  SI4432无线模块配置,根据不同的射频参数配置射频模块；
  +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
void Si4432::set_config()
{
	ItStatus1 = SpiWriteRegister(0x03,0x00);		// 清中断
	ItStatus2 = SpiWriteRegister(0x04,0x00);
	
	SpiWriteRegister(0x06, 0x00);  			
	
	SpiWriteRegister(0x07, 1);		// 进ready模式

	SpiWriteRegister(0x09, 0x7f);  	// 电容 12P

	SpiWriteRegister(0x0a, 0x05);	
	SpiWriteRegister(0x0b, 0xea); 	// GPIO 0 数据口输出（发射天线开关控制）
	SpiWriteRegister(0x0c, 0xea); 	// GPIO 1 数据口输出（接收天线开关控制）
	
	SpiWriteRegister(0x0d, 0xf4);  	// GPIO 2 输出接收数据
	
	SpiWriteRegister(0x70, 0x2c);  
	SpiWriteRegister(0x1d, 0x40);  	// 使能 afc
		
	// 1.2K bps setting
	SpiWriteRegister(0x1c, 0x16);	//空中速率设为1.2K
	SpiWriteRegister(0x20, 0x83); 	 
	SpiWriteRegister(0x21, 0xc0); 
	SpiWriteRegister(0x22, 0x13); 
	SpiWriteRegister(0x23, 0xa9); 
	SpiWriteRegister(0x24, 0x00); 
	SpiWriteRegister(0x25, 0x04);	
	SpiWriteRegister(0x2a, 0x14);
	SpiWriteRegister(0x6e, 0x09);
	SpiWriteRegister(0x6f, 0xd5);	
	//1.2K bps setting end		
	
	SpiWriteRegister(0x30, 0x8c);	// 使能 PH+ FIFO模式, 使能 crc, MSB					
	SpiWriteRegister(0x32, 0xff);	 
	SpiWriteRegister(0x33, 0x42);	// 头码 = byte 0,1,2,3, 同步字 = byte 3, 2
				
	SpiWriteRegister(0x34, 16);  	// 16个nibble前导码
	SpiWriteRegister(0x35, 0x20);  	// 有效检测20位前导码
	SpiWriteRegister(0x36, 0x2d);  	// 同步字 =  0x2dd4
	SpiWriteRegister(0x37, 0xd4);
	SpiWriteRegister(0x38, 0x00);
	SpiWriteRegister(0x39, 0x00);
	SpiWriteRegister(0x3a, 's');  	// 发射包头码 ：“swwx"
	SpiWriteRegister(0x3b, 'w');
	SpiWriteRegister(0x3c, 'w');
	SpiWriteRegister(0x3d, 'x');
	SpiWriteRegister(0x3e, 10);  	// 包长 = 10字节
	SpiWriteRegister(0x3f, 's'); 	// 接收校验头码 ：”swwx"
	SpiWriteRegister(0x40, 'w');
	SpiWriteRegister(0x41, 'w');
	SpiWriteRegister(0x42, 'x');
	SpiWriteRegister(0x43, 0xff);  	// 校验4字节头码 
	SpiWriteRegister(0x44, 0xff);  	
	SpiWriteRegister(0x45, 0xff); 	 
	SpiWriteRegister(0x46, 0xff);  	
	SpiWriteRegister(0x6d, 0x07);  	// 最大输出功率

	SpiWriteRegister(0x79, 0x0);  	// 
	SpiWriteRegister(0x7a, 0x0);  	// 无跳频	
	
	SpiWriteRegister(0x71, 0x22); 	// FiFo, FSK , not need clk			
	SpiWriteRegister(0x72, 0x20);  	// 频偏 20KHz	

	SpiWriteRegister(0x73, 0x0);  	
	SpiWriteRegister(0x74, 0x0);  	
		
	SpiWriteRegister(0x75, 0x53);  	// 频率:  433.5 MHz
	SpiWriteRegister(0x76, 0x57);  
	SpiWriteRegister(0x77, 0x80);	
}

//------------------------------------------------------------------------------------------------
// 函数名: set_rx_mode
// Notes : 设置模块工作在接收状态下，当一切准备好的时候，调用该函数可以让模块工作于接收模式下
//-----------------------------------------------------------------------------------------------
void Si4432::set_rx_mode(void)
{
    //read interrupt status registers to release all pending interrupts
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);	    //read the Interrupt Status2 register

    //Set the Rx switcher，利用GPIO口控制模块内部的天线开关。
    RxGPIOSetting();

    /*enable receiver chain*/
    SpiWriteRegister(0x07, 0x05);
	
	//使能接收数据包中断
	SpiWriteRegister(0x05,2);
}

//------------------------------------------------------------------------------------------------
// 函数名: set_tx_mode
// Notes : 设置模块工作在接收状态下，当一切准备好的时候，调用该函数可以让模块工作于发射状态模式下
//-----------------------------------------------------------------------------------------------
void Si4432::set_tx_mode(void)
{
    //Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
    ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register

    //Set the Rx switcher，利用GPIO口控制模块内部的天线开关。
    TxGPIOSetting();

    /*enable transmitter*/
    //The radio forms the packet and send it automatically.
    SpiWriteRegister(0x07, 0x09);
}

//------------------------------------------------------------------------------------------------
// 函数名: set_idle_mode
// Notes : 设置模块工作在空闲状态下。
//          当模块不需要接收数据也不需要发射数据的时候可以调用该函数以便省电
//-----------------------------------------------------------------------------------------------
void Si4432::set_idle_mode(void)
{
    //The radio will work in idle mode
    SpiWriteRegister(0x07, 0x01);	//write 0x09 to the Operating Function Control 1 register

    IdleGPIOSetting();

    //Read interrupt status regsiters. It clear all pending interrupts and the nIRQ pin goes back to high.
    ItStatus1 = SpiReadRegister(0x03);			//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);			//read the Interrupt Status2 register
}

//复位模块
void Si4432::reset(void)
{
	SpiWriteRegister(0x08, 0x02);												//write 0x02 to the Operating Function Control 2 register
	SpiWriteRegister(0x08, 0x00);												//write 0x00 to the Operating Function Control 2 register
}
//------------------------------------------------------------------------------------------------
// 函数名:write
// 参数  : uint8_t len_tx  : 发送数据长度(不大于64字节)
//         uint8_t *txdata: 内容的指针
// Notes : 数据发送
//-----------------------------------------------------------------------------------------------
void Si4432::write(uint8_t *txdata, uint8_t len_tx)
{
    uint8_t i;

    set_idle_mode();
	
    //设置数据包的长度(字节)
    SpiWriteRegister(0x3E, len_tx);

    //将数据写入RF
    for(i = 0; i < len_tx; i++)
    {
        SpiWriteRegister(0x7F, txdata[i]);
    }

	//关闭所有其他中断,仅打开发送中断。
    SpiWriteRegister(0x05, 0x04);	//write 0x04 to the Interrupt Enable 1 register
    SpiWriteRegister(0x06, 0x00);	//write 0x00 to the Interrupt Enable 2 register

    set_tx_mode();

    //等待发射完成中断的到来，如果中断采用非查询方式，则本程序到这里可以结束，但必须在中断服务程序中读取状态寄存器0x03和0x04，以便释放中断
    while (irq->read());

    //read interrupt status registers to release the interrupt flags
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register
}

//------------------------------------------------------------------------------------------------
// 函数名:read(uint8_t payload[])
// Notes : 接收数据,当中断来时
//-----------------------------------------------------------------------------------------------
uint8_t Si4432::read(uint8_t *rxbuf)
{	
	uint8_t temp8,len_Rx;

    //判断是否由于CRC校验出错引发的中断；在RFSetIdleMode中已经读出了中断状态寄存器的值
    if( (ItStatus1 & 0x01) == 0x01 )
    {
        //如果是CRC校验出错，那么 接收FIFO 复位；
        reset();
    }

    //判断是否是数据包已经被正确接收。
    if( (ItStatus1 & 0x02) == 0x02 )
    {
        len_Rx = SpiReadRegister(0x4B);		//读取收到的数据包长度
        //根据长度判断相应的操作。
        if(len_Rx <= 64)
        {
            //直接从FIFO中读取收到的数据。客户只要读出FIFO的数据就算收到数据。
            for(temp8 = 0; temp8 < len_Rx; temp8++)
            {
                rxbuf[temp8] = SpiReadRegister(0x7F);						//read the FIFO Access register
            }
        }
    }
	return len_Rx;
}

// 接收信号强度
uint8_t Si4432::read_rssi(void)
{
	return SpiReadRegister(0x26);
}

// 启动无线模块
void Si4432::start(void)
{
	//启动射频模块，模块的SDN引脚拉低后必须延时至少30ms，实际上可以直接把SDN引脚接地，这样就不用在程序中初始化了；
	ce->reset();
	
    delay_ms(30);
		
    //分别配置GPIO0/1/2端口为直接数字输出
    SpiWriteRegister(0x0B, 0xCA);
    SpiWriteRegister(0x0C, 0xCA);
    SpiWriteRegister(0x0D, 0xCA);

    //读取中断状态寄存器清除中断标志以便释放NIRQ引脚
    //如果中断来了，那么NIRQ引脚会被拉低；如果这时候中断状态寄存器0x03和0x04被读取，那么NIRQ引脚会被释放恢复高电平
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register

    //SW reset，软件复位这个模块主控芯片
    SpiWriteRegister(0x07, 0x80);			//write 0x80 to the Operating & Function Control1 register
    
	//等待软复位完成，当软复位完成后发生中断,读中断状态寄存器释放中断引脚(NIRQ引脚恢复高电平)
    while (irq->read());
	
    ItStatus1 = SpiReadRegister(0x03);		//read the Interrupt Status1 register
    ItStatus2 = SpiReadRegister(0x04);		//read the Interrupt Status2 register
}

uint8_t Si4432::read_irq(void)
{
	return irq->read();
}

