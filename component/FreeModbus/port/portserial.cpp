/*
 * FreeModbus Libary: BARE Port
 * Copyright (C) 2006 Christian Walter <wolti@sil.at>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: portserial.c,v 1.1 2006/08/22 21:35:13 wolti Exp $
 */

#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
#include "freemodbus.h"

/* ----------------------- static functions ---------------------------------*/
static void prvvUARTTxReadyISR( void );
static void prvvUARTRxISR( void );
static void USART_RXIRQ(void);
static void USART_TXIRQ(void);	

uint8_t TxEnaleBit=1;
uint8_t RxEnaleBit=1;
/* ----------------------- Start implementation -----------------------------*/
/**
  * @brief  控制接收和发送状态
  * @param  xRxEnable 接收使能、
  *         xTxEnable 发送使能
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
  if(xRxEnable)
  {
    //使能接收和接收中断
    //MAX485操作 低电平为接收模式
		RxEnaleBit=1;
		modbus.Mode4851->write(0);
		modbus.Mode4852->write(0);
  }
  else
  {
    //MAX485操作 高电平为发送模式
		RxEnaleBit=0;
		modbus.Mode4851->write(1);
		modbus.Mode4852->write(1);
  }

  if(xTxEnable)
  {
    //使能发送完成中断
		TxEnaleBit=1;
  }
  else
  {
    //禁止发送完成中断
		TxEnaleBit=0;
  }
  
}

/**
  * @brief  串口初始化
  * @param  ucPORT      串口号
  *         ulBaudRate  波特率
  *         ucDataBits  数据位
  *         eParity     校验位 
  * @retval None
  */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
  (void)ucPORT;     //不修改串口
  (void)ucDataBits; //不修改数据位长度
  (void)eParity;    //不修改校验格式	
	modbus.uart->begin(ulBaudRate); 		
	modbus.uart->attach(USART_RXIRQ,RxIrq);
	modbus.uart->attach(USART_TXIRQ,TcIrq);
	//最后配置485发送和接收模式
	modbus.Mode4851->mode(OUTPUT_PP);
	modbus.Mode4852->mode(OUTPUT_PP);
  return TRUE;
}

/**
  * @brief  通过串口发送数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
  //发送数据
	modbus.uart->write(ucByte);
  return TRUE;
}

/**
  * @brief  从串口获得数据
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
  //接收数据
	*pucByte = modbus.uart->read();
  return TRUE;
}

/* Create an interrupt handler for the transmit buffer empty interrupt
 * (or an equivalent) for your target processor. This function should then
 * call pxMBFrameCBTransmitterEmpty( ) which tells the protocol stack that
 * a new character can be sent. The protocol stack will then call 
 * xMBPortSerialPutByte( ) to send the character.
 */
static void prvvUARTTxReadyISR( void )
{
  //mb.c eMBInit函数中
  //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
  //发送状态机
  pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
  //mb.c eMBInit函数中
  //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
  //接收状态机
  pxMBFrameCBByteReceived();
}

u8 flag_rx = 0;

/**
  * @brief  USART中断服务函数
  * @param  None
  * @retval None
  */
void USART_RXIRQ(void)
{
	
  //发生接收中断
	if(RxEnaleBit)
	 prvvUARTRxISR();	
  }
	
void USART_TXIRQ(void)
{
	  //发生发送中断
	if(TxEnaleBit)
   prvvUARTTxReadyISR();
  }
 

