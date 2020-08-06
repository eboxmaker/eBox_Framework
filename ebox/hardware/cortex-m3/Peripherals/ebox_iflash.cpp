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
#if USE_EBOX_MEM
#include "ebox_mem.h"
#endif



#if EBOX_DEBUG
// �Ƿ��ӡ������Ϣ, 1��ӡ,0����ӡ
#define debug 1
#endif

#if debug
#define  IFLASH_DEBUG(...) DBG("[IFLASH]  "),DBG(__VA_ARGS__)
#else
#define  IFLASH_DEBUG(...)
#endif


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

__INLINE  uint16_t write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;


    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
        FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t *)buf);
        i = i + 2;
        iAddress = iAddress + 2;
        buf = buf + 2;
    }


    return iNumByteToWrite;
}

Flash::Flash()
{
    //MCU_FLASH_PRG_END  +  FLASH_PAGE_SIZE
    _start_addr = MCU_FLASH_PRG_END + (FLASH_PAGE_SIZE - MCU_FLASH_PRG_END % FLASH_PAGE_SIZE);
    _end_addr = FLASH_USER_END_ADDR;
}

/**
 *@brief    ���캯��,�趨��flash��С = nPage * FLASH_PAGE_SIZE
 *@param    uint8_t startAddr  �ӵڼ���������ʼ
		   uint8_t nPage		ҳ���� > 0
 *@retval   E_FlashStates
*/
Flash::Flash(uint8_t startAddr, uint8_t nPage)
{
    // ��ʼ��flash��ʼ,������ַ
    if ((FLASH_USER_START_ADDR + startAddr * FLASH_PAGE_SIZE) <= (FLASH_USER_END_ADDR - FLASH_PAGE_SIZE))
    {
        _start_addr = FLASH_USER_START_ADDR + startAddr * FLASH_PAGE_SIZE;
    }
    else
    {
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
    }
    else
    {
        _end_addr = (_start_addr + nPage * FLASH_PAGE_SIZE) ;
    }
}

/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to write into flash
  * @retval if success return the number to write, -1 if error
  *
  */
#if (FLASH_OVERRIDE == 0)  // ֻ����Ҫд�������,��ǰ�����������ݱ��ֲ���
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos, iAddress = _start_addr + offsetAdd;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i = 0;
    // ���建���������������ҳ����
#if USE_EBOX_MEM
    uint8_t *tmp = (uint8_t *)ebox_malloc(FLASH_PAGE_SIZE);
#else
    static uint8_t tmp[FLASH_PAGE_SIZE];
#endif

    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    
    //if ((_start_addr == 0) || (iAddress >= _end_addr))
    {
        IFLASH_DEBUG("write: flash prg end��0x%x , page size = 0x%x\r\n", MCU_FLASH_PRG_END,FLASH_PAGE_SIZE);

        IFLASH_DEBUG("write: flash �������������ʼ��ַ��ҳ���� s��0x%x e:0x%x,i:0x%x\r\n", _start_addr, _end_addr, iAddress);
       // return 0;
    }
    FLASH_UnlockBank1();

    secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //������ַ
    secoff = iAddress & (FLASH_PAGE_SIZE - 1);  //�������ڵ�ƫ��
    secremain = FLASH_PAGE_SIZE - secoff;       //����ʣ��ռ��С
    IFLASH_DEBUG("secpos = 0x%x, secoff = 0x%x, secremain = 0x%x \r\n", secpos, secoff, secremain);
    if(iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

    while( 1 )
    {
        ebox_flashRead(secpos, tmp, FLASH_PAGE_SIZE);   //������������
        for(i = 0; i < secremain; i++)   //У������
        {
            if(tmp[secoff + i] != 0XFF)break;   //��Ҫ����
        }
        if(i < secremain)  //��Ҫ����
        {
            FLASHStatus = FLASH_ErasePage(secpos); //�����������
            if(FLASHStatus != FLASH_COMPLETE)
                return -1;
            for(i = 0; i < secremain; i++) //����
            {
                tmp[i + secoff] = buf[i];
            }
            write_without_check(secpos, tmp, FLASH_PAGE_SIZE);   //д����������
        }
        else
        {
            write_without_check(iAddress, buf, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        }

        if(iNumByteToWrite == secremain)  //д�������
        {
            break;
        }
        else     //����д�룬�Ƶ���һ������
        {
            secpos += FLASH_PAGE_SIZE;
            if (secpos >= _end_addr)
            {
                IFLASH_DEBUG("write: �ռ䲻�㣬����flash��С��\r\n");
                return -1;
            }
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain;  //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain;  //�ֽ����ݼ�
            if(iNumByteToWrite > FLASH_PAGE_SIZE) secremain = FLASH_PAGE_SIZE; //��һ����������д����
            else secremain = iNumByteToWrite;  //��һ����������д����
        }

    }
#if USE_EBOX_MEM
    ebox_free(tmp);
#endif
    FLASH_LockBank1();
    return iNbrToWrite;
}
#else
// ����ԭ�����ݣ�д��������
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
    uint32_t secpos, iAddress = _start_addr + offsetAdd;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;

    if ((_start_addr == 0) || (iAddress == _end_addr))
    {
        IFLASH_DEBUG("write: flash �������������ʼ��ַ��ҳ����\r\n");
        return 0;
    }
    FLASH_UnlockBank1();
    secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //������ַ(ȡ12-31bitλ)
    secoff = iAddress & (FLASH_PAGE_SIZE - 1);  	//�������ڵ�ƫ��(0-11bitλ)
    secremain = FLASH_PAGE_SIZE - secoff;       	//����ʣ��ռ��С
    // ��ǰ����ʣ��ռ����д������
    if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

    while ( 1 )
    {
        FLASH_ErasePage(secpos); //�����������
        write_without_check(iAddress, buf, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.

        // ��ǰ����ʣ��ռ乻д��
        if (iNumByteToWrite == secremain) //д�������
            break;
        else	//����д�룬�Ƶ���һ������
        {
            secpos += FLASH_PAGE_SIZE;
            if (secpos >= _end_addr)
            {
                IFLASH_DEBUG("write: �ռ䲻�㣬����flash��С��\r\n");
                return -1;
            }
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain;  //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain;  //�ֽ����ݼ�
            if (iNumByteToWrite > FLASH_PAGE_SIZE)
            {
                secremain = FLASH_PAGE_SIZE; //��һ����������д����
            }
            else
            {
                secremain = iNumByteToWrite;  //��һ����������д����
            }
        }
    }
    FLASH_LockBank1();
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
    if (_start_addr == 0)
    {
        IFLASH_DEBUG("read: flash �������������ʼ��ַ��ҳ����\r\n");
        return 0;
    }
    return ebox_flashRead(iAddress, buf, iNbrToRead);
}

uint32_t Flash::getSize()
{
    return (_end_addr - _start_addr) / 1024;
}



