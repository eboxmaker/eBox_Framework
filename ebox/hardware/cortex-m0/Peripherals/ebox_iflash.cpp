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
// 是否打印调试信息, 1打印,0不打印
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
	FLASH_BUSY = 1,			//忙
	FLASH_ERROR_WRP,		//写保护错误
	FLASH_ERROR_PROGRAM,	//编程错误
	FLASH_COMPLETE,			//结束
	FLASH_TIMEOUT			//超时
}E_FlashStates;

#define FLASH_BSY             FLASH_SR_BSY            /*!< FLASH Busy flag                           */
#define FLASH_PGERR           FLASH_SR_PGERR          /*!< FLASH Programming error flag    */
#define FLASH_WRPERR          FLASH_SR_WRPERR         /*!< FLASH Write protected error flag          */
#define FLASH_EOP             FLASH_SR_EOP            /*!< FLASH End of Operation flag               */

#define FLASH_GET_FLAG(__FLAG__)   (((FLASH->SR) & (__FLAG__)) == (__FLAG__))
#define FLASH_CLEAR_FLAG(__FLAG__)   ((FLASH->SR) = (__FLAG__))

/**
 *@brief    获取flash状态
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
 *@brief    忙等待
 *@param    uint16_t timeout 超时时间
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
 *@brief    解锁
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
 *@brief    加锁
 *@param    NONE
 *@retval   NONE
*/
void ebox_FLASH_Lock(void)
{
	/* Set the LOCK Bit to lock the FLASH Registers access */
	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
}
/**
 *@brief    halfword写入
 *@param    uint32_t Address 要写入的地址
		   uint16_t Data 	 要写入的数据
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
 *@brief    页擦除
 *@param    uint32_t PageAddress 页地址
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
 *@brief    读指定长度数据
 *@param    uint32_t iAddress 起始地址
 *@retval   读到的数据长度
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
 *@brief    写入数据
 *@param    uint32_t iAddress 起始地址， uint8_t *buf,要写入的数据 uint16_t iNumByteToWrite 数据长度
 *@retval   写入的数据长度
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
 *@brief    构造函数,设定的flash大小 = nPage * FLASH_PAGE_SIZE
 *@param    uint8_t startAddr  从第几个扇区开始
		   uint8_t nPage		页面数 > 0
 *@retval   E_FlashStates
*/
Flash::Flash(uint8_t startAddr,uint8_t nPage){
	// 初始化flash起始,结束地址
	if ((FLASH_USER_START_ADDR + startAddr * FLASH_PAGE_SIZE)<=(FLASH_USER_END_ADDR-FLASH_PAGE_SIZE))
	{
		_start_addr = FLASH_USER_START_ADDR + startAddr * FLASH_PAGE_SIZE;
	}else{
		_start_addr = 0;
	}
	// 页面数不能为0
	if (nPage == 0)
	{
		_start_addr = 0;
		return;
	}
	// 计算结束地址
	if ((_start_addr + nPage * FLASH_PAGE_SIZE) > FLASH_USER_END_ADDR)
	{
		_end_addr = FLASH_USER_END_ADDR ;
	}else{
		_end_addr = (_start_addr + nPage * FLASH_PAGE_SIZE) ;
	}
}
/**
 *@brief    写入一组数据
 *@param    uint32_t offsetAdd  	要写入的地址
		   uint8_t *buf			要写入的数据
		   uint32_t iNbrToWrite	要写入的数据长度
 *@retval   写入的数据长度。 如果 0 错误
*/
#define FLASH_PAGE_S FLASH_PAGE_SIZE

#if (FLASH_OVERRIDE == 0)  // 只更新要写入的数据,当前区块其他数据保持不变
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
	uint32_t secpos,iAddress = _start_addr + offsetAdd;
	uint32_t iNumByteToWrite = iNbrToWrite;
	uint16_t secoff;
	uint16_t secremain;
	uint16_t i = 0;
	// 定义缓冲区，保存读出的页数据
	static uint8_t tmp[FLASH_PAGE_S];

	if ((_start_addr == 0)||(iAddress == _end_addr)){
		IFLASH_DEBUG("write: flash 定义错误，请检查起始地址和页数量 s：%d e:%d\r\n",_start_addr,_end_addr);
		return 0;
	}

	secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //扇区地址(取12-31bit位)
	secoff = iAddress & (FLASH_PAGE_SIZE - 1);  	//在扇区内的偏移(0-11bit位)
	secremain = FLASH_PAGE_SIZE - secoff;       	//扇区剩余空间大小
	// 当前扇区剩余空间可以写完数据
	if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //不大于4096个字节

	while ( 1 )
	{
		ebox_flashRead(secpos, tmp, FLASH_PAGE_S);   //读出整个扇区
		// 判断需要写入的部分是否为空
		for (i = 0; i < secremain; i++)  	//校验数据
		{
			if (tmp[secoff + i] != 0XFF)break;  //需要擦除
		}
		if (i < secremain) //需要擦除
		{
			ebox_FLASH_PageErase(secpos); //擦除这个扇区
			// 将需要写入的数据写入缓冲区
			for (i = 0; i < secremain; i++) //复制
			{
				tmp[i + secoff] = buf[i];
			}
			// 将缓冲区数据写入扇区
			write_without_check(secpos , tmp , FLASH_PAGE_S); //写入整个扇区
		}
		else
		{
			write_without_check(iAddress, buf, secremain); //写已经擦除了的,直接写入扇区剩余区间.
		}
		// 当前扇区剩余空间够写入
		if (iNumByteToWrite == secremain) //写入结束了
			break;
		else	//不够写入，移到下一个扇区
		{
			secpos += FLASH_PAGE_SIZE;
			if (secpos >= _end_addr){
				IFLASH_DEBUG("write: 空间不足，请检查flash大小！\r\n");
				return -1;
			}
			secoff = 0;//偏移位置为0
			buf += secremain;  //指针偏移
			iAddress += secremain;//写地址偏移
			iNumByteToWrite -= secremain;  //字节数递减
			if (iNumByteToWrite > FLASH_PAGE_SIZE){
				secremain = FLASH_PAGE_SIZE; //下一个扇区还是写不完
			}else{
				secremain = iNumByteToWrite;  //下一个扇区可以写完了
			}
		}
	}
	return iNbrToWrite;
}
#else
// 擦除原有数据，写入新数据
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
	uint32_t secpos,iAddress = _start_addr + offsetAdd;
	uint32_t iNumByteToWrite = iNbrToWrite;
	uint16_t secoff;
	uint16_t secremain;

	if ((_start_addr == 0)||(iAddress == _end_addr)){
		IFLASH_DEBUG("write: flash 定义错误，请检查起始地址和页数量\r\n");
		return 0;
	}

	secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //扇区地址(取12-31bit位)
	secoff = iAddress & (FLASH_PAGE_SIZE - 1);  	//在扇区内的偏移(0-11bit位)
	secremain = FLASH_PAGE_SIZE - secoff;       	//扇区剩余空间大小
	// 当前扇区剩余空间可以写完数据
	if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //不大于4096个字节

	while ( 1 )
	{
		ebox_FLASH_PageErase(secpos); //擦除这个扇区
		write_without_check(iAddress, buf, secremain); //写已经擦除了的,直接写入扇区剩余区间.

		// 当前扇区剩余空间够写入
		if (iNumByteToWrite == secremain) //写入结束了
			break;
		else	//不够写入，移到下一个扇区
		{
			secpos += FLASH_PAGE_SIZE;
			if (secpos >= _end_addr){
				IFLASH_DEBUG("write: 空间不足，请检查flash大小！\r\n");
				return -1;
			}
			secoff = 0;//偏移位置为0
			buf += secremain;  //指针偏移
			iAddress += secremain;//写地址偏移
			iNumByteToWrite -= secremain;  //字节数递减
			if (iNumByteToWrite > FLASH_PAGE_SIZE){
				secremain = FLASH_PAGE_SIZE; //下一个扇区还是写不完
			}else{
				secremain = iNumByteToWrite;  //下一个扇区可以写完了
			}
		}
	}
	return iNbrToWrite;
}
#endif

/**
 *@brief    读出一组数据
 *@param    uint32_t offsetAdd  	要读取的地址
		   uint8_t *buf			保存读取的数据
		   uint32_t iNbrToWrite	要读取的数据长度
 *@retval   读出的数据长度。 如果 0 错误
*/
int Flash::read(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToRead)
{
	// 绝对地址
	uint32_t iAddress = _start_addr + offsetAdd;

	if (_start_addr == 0){
		IFLASH_DEBUG("read: flash 定义错误，请检查起始地址和页数量\r\n");
		return 0;
	}

	return ebox_flashRead(iAddress,buf,iNbrToRead);
}



