/**
  ******************************************************************************
  * @file    iflash.cpp
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/
#include "ebox_iflash.h"
#include "stm32f0xx_hal_flash_ex.h"

#if USE_PRINTF
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 0
#endif

#if debug
#include "ebox_uart.h"
extern Uart uart1;
#define DBG(...) uart1.printf(__VA_ARGS__)
#define  IFLASH_DEBUG(...) DBG("[IFLASH]  "),DBG(__VA_ARGS__)
#else
#define  IFLASH_DEBUG(...)
#endif


typedef enum
{
	FLASH_BUSY = 1,			//æ
	FLASH_ERROR_WRP,		//д��������
	FLASH_ERROR_PROGRAM,	//��̴���
	FLASH_COMPLETE,			//����
	FLASH_TIMEOUT			//��ʱ
}E_FlashStates;

#define FLASH_BSY             FLASH_SR_BSY            /*!< FLASH Busy flag                           */
#define FLASH_PGERR           FLASH_SR_PGERR          /*!< FLASH Programming error flag    */
#define FLASH_WRPERR          FLASH_SR_WRPERR         /*!< FLASH Write protected error flag          */
#define FLASH_EOP             FLASH_SR_EOP            /*!< FLASH End of Operation flag               */

#define FLASH_GET_FLAG(__FLAG__)   (((FLASH->SR) & (__FLAG__)) == (__FLAG__))
#define FLASH_CLEAR_FLAG(__FLAG__)   ((FLASH->SR) = (__FLAG__))

/**
 *@brief    ��ȡflash״̬
 *@param    none
 *@retval   E_FlashStates
*/
E_FlashStates ebox_getFlashStates(void){
	if (FLASH_GET_FLAG(FLASH_BSY)) 				return FLASH_BUSY;
	if (FLASH_GET_FLAG(FLASH_PGERR))			return FLASH_ERROR_PROGRAM;
	if (FLASH_GET_FLAG(FLASH_SR_WRPERR))	return FLASH_ERROR_WRP;
	if (FLASH_GET_FLAG(FLASH_SR_EOP))			return FLASH_COMPLETE;
	return FLASH_COMPLETE;
}

/**
 *@brief    æ�ȴ�
 *@param    uint16_t timeout ��ʱʱ��
 *@retval   E_FlashStates
*/
E_FlashStates ebox_waiteBusy(uint16_t timeout){
	E_FlashStates state = FLASH_COMPLETE;

	do{
		state = ebox_getFlashStates();
	}while ((state == FLASH_BUSY)&&(timeout-- != 0));

	if (timeout == 0)
	{
		state = FLASH_TIMEOUT;
	}
	return state;
}

/**
 *@brief    ����
 *@param    NONE
 *@retval   NONE
*/
void ebox_FLASH_Unlock(void)
{
	if (((FLASH->CR) & (FLASH_CR_LOCK)) != RESET)
	{
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);
	}
}
/**
 *@brief    ����
 *@param    NONE
 *@retval   NONE
*/
void ebox_FLASH_Lock(void)
{
	/* Set the LOCK Bit to lock the FLASH Registers access */
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}
/**
 *@brief    halfwordд��
 *@param    uint32_t Address Ҫд��ĵ�ַ
		   uint16_t Data 	 Ҫд�������
 *@retval   E_FlashStates
*/
E_FlashStates ebox_Program_HalfWord(uint32_t Address, uint16_t Data)
{
	E_FlashStates state = FLASH_COMPLETE;
	state = ebox_waiteBusy(1000);

	if (state == FLASH_COMPLETE)
	{
		/* If the previous operation is completed, proceed to program the new data */
		FLASH->CR |= FLASH_CR_PG;

		*(__IO uint16_t*)Address = Data;

		/* Wait for last operation to be completed */
		state = ebox_waiteBusy(1000);
		/* Disable the PG Bit */
		FLASH->CR &= ~FLASH_CR_PG;
	}
	return state;
}

/**
 *@brief    ҳ����
 *@param    uint32_t PageAddress ҳ��ַ
 *@retval   E_FlashStates
*/
E_FlashStates ebox_FLASH_PageErase(uint32_t PageAddress)
{
	E_FlashStates state = FLASH_COMPLETE;
	ebox_FLASH_Unlock();
	FLASH_CLEAR_FLAG(FLASH_BSY|FLASH_PGERR|FLASH_WRPERR|FLASH_EOP);
	state = ebox_waiteBusy(1000);
	if (state == FLASH_COMPLETE){
		/* Proceed to erase the page */
		SET_BIT(FLASH->CR, FLASH_CR_PER);
		WRITE_REG(FLASH->AR, PageAddress);
		SET_BIT(FLASH->CR, FLASH_CR_STRT);
		state = ebox_waiteBusy(1000);
		/* Disable the PER Bit */
		FLASH->CR &= ~FLASH_CR_PER;
	}
	ebox_FLASH_Lock();
#if debug
	IFLASH_DEBUG("PageErase state : ");
	switch(state){
		case 1:
			DBG("BUSY \r\n");
			break;
		case 2:
			DBG("ERROR_WRP \r\n");
			break;
		case 3:
			DBG("ERROR_PROGRAM \r\n");
			break;
		case 4:
		  DBG("COMPLETE \r\n");
			break;
		case 5:
			DBG("TIMEOUT \r\n");
			break;
	}
#endif
	return state;
}

/**
 *@brief    ��ָ����������
 *@param    uint32_t iAddress ��ʼ��ַ
 *@retval   ���������ݳ���
*/
__INLINE int ebox_flashRead(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToRead)
{
	int i = 0;
	while (i < iNbrToRead )
	{
		*(buf + i) = *(__IO uint8_t *) iAddress++;
		i++;
	}
	return i;
}

/**
 *@brief    д������
 *@param    uint32_t iAddress ��ʼ��ַ�� uint8_t *buf,Ҫд������� uint16_t iNumByteToWrite ���ݳ���
 *@retval   д������ݳ���
*/
__INLINE uint16_t write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
	uint16_t i;

	i = 0;

	ebox_FLASH_Unlock();

	while ((i < iNumByteToWrite)/* && (FLASH_GET_FLAG(FLASH_EOP))*/)
	{
		ebox_Program_HalfWord(iAddress, *(uint16_t *)buf);
		i = i + 2;
		iAddress = iAddress + 2;
		buf = buf + 2;
	}

	ebox_FLASH_Lock();

	return iNumByteToWrite;
}

Flash::Flash(){
    _start_addr = FLASH_USER_START_ADDR;
    _end_addr = FLASH_USER_END_ADDR;
}

/**
 *@brief    ���캯��,�趨��flash��С = nPage * FLASH_PAGE_SIZE
 *@param    uint8_t startAddr  �ӵڼ���������ʼ
		   uint8_t nPage		ҳ���� > 0
 *@retval   E_FlashStates
*/
Flash::Flash(uint8_t startAddr,uint8_t nPage){
	// ��ʼ��flash��ʼ,������ַ
	if ((FLASH_USER_START_ADDR + startAddr * FLASH_PAGE_SIZE)<=(FLASH_USER_END_ADDR-FLASH_PAGE_SIZE))
	{
		_start_addr = FLASH_USER_START_ADDR + startAddr * FLASH_PAGE_SIZE;
	}else{
		_start_addr = 0;
	}
	// ҳ��������Ϊ0
	if (nPage == 0)
	{
		_start_addr = 0;
		return;
	}
	// ���������ַ
	if ((_start_addr + nPage * FLASH_PAGE_SIZE) > FLASH_USER_END_ADDR)
	{
		_end_addr = FLASH_USER_END_ADDR ;
	}else{
		_end_addr = (_start_addr + nPage * FLASH_PAGE_SIZE) ;
	}
}
/**
 *@brief    д��һ������
 *@param    uint32_t offsetAdd  	Ҫд��ĵ�ַ
		   uint8_t *buf			Ҫд�������
		   uint32_t iNbrToWrite	Ҫд������ݳ���
 *@retval   д������ݳ��ȡ� ��� 0 ����
*/
#define FLASH_PAGE_S FLASH_PAGE_SIZE

#if (FLASH_OVERRIDE == 0)  // ֻ����Ҫд�������,��ǰ�����������ݱ��ֲ���
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
	uint32_t secpos,iAddress = _start_addr + offsetAdd;
	uint32_t iNumByteToWrite = iNbrToWrite;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i = 0;
	// ���建���������������ҳ����
	static uint8_t tmp[FLASH_PAGE_S];

	if ((_start_addr == 0)||(iAddress == _end_addr)){
		IFLASH_DEBUG("write: flash �������������ʼ��ַ��ҳ���� s��%d e:%d\r\n",_start_addr,_end_addr);
		return 0;
	}

	secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //������ַ(ȡ12-31bitλ)
	secoff = iAddress & (FLASH_PAGE_SIZE - 1);  	//�������ڵ�ƫ��(0-11bitλ)
	secremain = FLASH_PAGE_SIZE - secoff;       	//����ʣ��ռ��С
	// ��ǰ����ʣ��ռ����д������
	if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

	while ( 1 )
	{
		ebox_flashRead(secpos, tmp, FLASH_PAGE_S);   //������������
		// �ж���Ҫд��Ĳ����Ƿ�Ϊ��
		for (i = 0; i < secremain; i++)  	//У������
		{
			if (tmp[secoff + i] != 0XFF)break;  //��Ҫ����
		}
		if (i < secremain) //��Ҫ����
		{
			ebox_FLASH_PageErase(secpos); //�����������
			// ����Ҫд�������д�뻺����
			for (i = 0; i < secremain; i++) //����
			{
				tmp[i + secoff] = buf[i];
			}
			// ������������д������
			write_without_check(secpos , tmp , FLASH_PAGE_S); //д����������
		}
		else
		{
			write_without_check(iAddress, buf, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
		}
		// ��ǰ����ʣ��ռ乻д��
		if (iNumByteToWrite == secremain) //д�������
			break;
		else	//����д�룬�Ƶ���һ������
		{
			secpos += FLASH_PAGE_SIZE;
			if (secpos >= _end_addr){
				IFLASH_DEBUG("write: �ռ䲻�㣬����flash��С��\r\n");
				return -1;
			}
			secoff = 0;//ƫ��λ��Ϊ0
			buf += secremain;  //ָ��ƫ��
			iAddress += secremain;//д��ַƫ��
			iNumByteToWrite -= secremain;  //�ֽ����ݼ�
			if (iNumByteToWrite > FLASH_PAGE_SIZE){
				secremain = FLASH_PAGE_SIZE; //��һ����������д����
			}else{
				secremain = iNumByteToWrite;  //��һ����������д����
			}
		}
	}
	return iNbrToWrite;
}
#else
// ����ԭ�����ݣ�д��������
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
	uint32_t secpos,iAddress = _start_addr + offsetAdd;
	uint32_t iNumByteToWrite = iNbrToWrite;
	uint16_t secoff;
	uint16_t secremain;

	if ((_start_addr == 0)||(iAddress == _end_addr)){
		IFLASH_DEBUG("write: flash �������������ʼ��ַ��ҳ����\r\n");
		return 0;
	}

	secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //������ַ(ȡ12-31bitλ)
	secoff = iAddress & (FLASH_PAGE_SIZE - 1);  	//�������ڵ�ƫ��(0-11bitλ)
	secremain = FLASH_PAGE_SIZE - secoff;       	//����ʣ��ռ��С
	// ��ǰ����ʣ��ռ����д������
	if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

	while ( 1 )
	{
		ebox_FLASH_PageErase(secpos); //�����������
		write_without_check(iAddress, buf, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.

		// ��ǰ����ʣ��ռ乻д��
		if (iNumByteToWrite == secremain) //д�������
			break;
		else	//����д�룬�Ƶ���һ������
		{
			secpos += FLASH_PAGE_SIZE;
			if (secpos >= _end_addr){
				IFLASH_DEBUG("write: �ռ䲻�㣬����flash��С��\r\n");
				return -1;
			}
			secoff = 0;//ƫ��λ��Ϊ0
			buf += secremain;  //ָ��ƫ��
			iAddress += secremain;//д��ַƫ��
			iNumByteToWrite -= secremain;  //�ֽ����ݼ�
			if (iNumByteToWrite > FLASH_PAGE_SIZE){
				secremain = FLASH_PAGE_SIZE; //��һ����������д����
			}else{
				secremain = iNumByteToWrite;  //��һ����������д����
			}
		}
	}
	return iNbrToWrite;
}
#endif

/**
 *@brief    ����һ������
 *@param    uint32_t offsetAdd  	Ҫ��ȡ�ĵ�ַ
		   uint8_t *buf			�����ȡ������
		   uint32_t iNbrToWrite	Ҫ��ȡ�����ݳ���
 *@retval   ���������ݳ��ȡ� ��� 0 ����
*/
int Flash::read(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToRead)
{
	// ���Ե�ַ
	uint32_t iAddress = _start_addr + offsetAdd;

	if (_start_addr == 0){
		IFLASH_DEBUG("read: flash �������������ʼ��ַ��ҳ����\r\n");
		return 0;
	}

	return ebox_flashRead(iAddress,buf,iNbrToRead);
}



