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
  * @brief  ���ƽ��պͷ���״̬
  * @param  xRxEnable ����ʹ�ܡ�
  *         xTxEnable ����ʹ��
  * @retval None
  */
void
vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{
  if(xRxEnable)
  {
    //ʹ�ܽ��պͽ����ж�
    //MAX485���� �͵�ƽΪ����ģʽ
		RxEnaleBit=1;
		modbus.Mode4851->write(0);
		modbus.Mode4852->write(0);
  }
  else
  {
    //MAX485���� �ߵ�ƽΪ����ģʽ
		RxEnaleBit=0;
		modbus.Mode4851->write(1);
		modbus.Mode4852->write(1);
  }

  if(xTxEnable)
  {
    //ʹ�ܷ�������ж�
		TxEnaleBit=1;
  }
  else
  {
    //��ֹ��������ж�
		TxEnaleBit=0;
  }
  
}

/**
  * @brief  ���ڳ�ʼ��
  * @param  ucPORT      ���ں�
  *         ulBaudRate  ������
  *         ucDataBits  ����λ
  *         eParity     У��λ 
  * @retval None
  */
BOOL
xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
  (void)ucPORT;     //���޸Ĵ���
  (void)ucDataBits; //���޸�����λ����
  (void)eParity;    //���޸�У���ʽ	
	modbus.uart->begin(ulBaudRate); 		
	modbus.uart->attach(USART_RXIRQ,RxIrq);
	modbus.uart->attach(USART_TXIRQ,TcIrq);
	//�������485���ͺͽ���ģʽ
	modbus.Mode4851->mode(OUTPUT_PP);
	modbus.Mode4852->mode(OUTPUT_PP);
  return TRUE;
}

/**
  * @brief  ͨ�����ڷ�������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialPutByte( CHAR ucByte )
{
  //��������
	modbus.uart->write(ucByte);
  return TRUE;
}

/**
  * @brief  �Ӵ��ڻ������
  * @param  None
  * @retval None
  */
BOOL
xMBPortSerialGetByte( CHAR * pucByte )
{
  //��������
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
  //mb.c eMBInit������
  //pxMBFrameCBTransmitterEmpty = xMBRTUTransmitFSM 
  //����״̬��
  pxMBFrameCBTransmitterEmpty();
}

/* Create an interrupt handler for the receive interrupt for your target
 * processor. This function should then call pxMBFrameCBByteReceived( ). The
 * protocol stack will then call xMBPortSerialGetByte( ) to retrieve the
 * character.
 */
static void prvvUARTRxISR( void )
{
  //mb.c eMBInit������
  //pxMBFrameCBByteReceived = xMBRTUReceiveFSM
  //����״̬��
  pxMBFrameCBByteReceived();
}

u8 flag_rx = 0;

/**
  * @brief  USART�жϷ�����
  * @param  None
  * @retval None
  */
void USART_RXIRQ(void)
{
	
  //���������ж�
	if(RxEnaleBit)
	 prvvUARTRxISR();	
  }
	
void USART_TXIRQ(void)
{
	  //���������ж�
	if(TxEnaleBit)
   prvvUARTTxReadyISR();
  }
 

