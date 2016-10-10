/**
  ******************************************************************************
  * @file    iflash.cpp
  * @author  shentq
  * @version V1.2
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




uint16_t Flash::write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;
    i = 0;

    //    FLASH_UnlockBank1();
    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
        FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t *)buf);
        i = i + 2;
        iAddress = iAddress + 2;
        buf = buf + 2;
    }

    return iNumByteToWrite;
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

int Flash::write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i = 0;
    static uint8_t tmp[FLASH_PAGE_SIZE];
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    FLASH_UnlockBank1();
    secpos = iAddress & (~(FLASH_PAGE_SIZE - 1 )) ; //������ַ
    secoff = iAddress & (FLASH_PAGE_SIZE - 1);  //�������ڵ�ƫ��
    secremain = FLASH_PAGE_SIZE - secoff;       //����ʣ��ռ��С

    if(iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

    while( 1 )
    {
        read(secpos, tmp, FLASH_PAGE_SIZE);   //������������
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
            write_without_check(secpos , tmp , FLASH_PAGE_SIZE); //д����������
        }
        else
        {
            write_without_check(iAddress, buf, secremain); //д�Ѿ������˵�,ֱ��д������ʣ������.
        }

        if(iNumByteToWrite == secremain) //д�������
            break;
        else
        {
            secpos += FLASH_PAGE_SIZE;
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain;  //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain;  //�ֽ����ݼ�
            if(iNumByteToWrite > FLASH_PAGE_SIZE) secremain = FLASH_PAGE_SIZE; //��һ����������д����
            else secremain = iNumByteToWrite;  //��һ����������д����
        }

    }

    FLASH_LockBank1();
    return iNbrToWrite;
}






/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to read from flash
  * @retval if success return the number to write, without error
  *
  */
int Flash::read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead)
{
    int i = 0;
    while(i < iNbrToRead )
    {
        *(buf + i) = *(__IO uint8_t *) iAddress++;
        i++;
    }
    return i;
}

