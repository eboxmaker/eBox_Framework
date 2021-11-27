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
#define EBOX_DEBUG_IFLASH_ENABLE       false
#define EBOX_DEBUG_IFLASH_ENABLE_ERR   true
#endif


#if EBOX_DEBUG_IFLASH_ENABLE
#define iflashDebug(...)  ebox_printf("[iflash DBG]:%d: ",__LINE__),ebox_printf(__VA_ARGS__ )
#else
#define iflashDebug(...)
#endif

#if EBOX_DEBUG_IFLASH_ENABLE_ERR
#define iflashDebugErr(fmt, ...)  ebox_printf("[iflash err]:%d: " fmt "\n", __LINE__, __VA_ARGS__)
#else
#define iflashDebugErr(fmt, ...)
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
 
}
int AddrToPage(uint32_t addr)
{
    return (addr - cpu.flash.start)/cpu.flash.page_size;
}
bool Flash::begin(int nPage)
{
    
    
    //MCU_FLASH_PRG_END  +  FLASH_PAGE_SIZE
    uint8_t total_page = cpu.flash.size / cpu.flash.page_size;
    uint32_t page_pgm_end = cpu.flash.used / cpu.flash.page_size;
    uint32_t page_remained = total_page - 1 - page_pgm_end;
    if(page_remained <= 0)
    {
        iflashDebugErr("û���㹻�Ŀռ�page_remained=%d\n",page_remained);
        return false;
    }
    uint32_t page_user_start = 1 + page_pgm_end;
    float used_page = cpu.flash.used * 1.0 / cpu.flash.page_size;
    
    if(nPage > page_remained)
    {
        nPage = page_remained;
    }
    if(nPage < 0)
    {
        nPage = 1;
    }
    _end_addr =  cpu.flash.end;
    _start_addr =  _end_addr - nPage * cpu.flash.page_size + 1; 
    
    iflashDebug("is_ok:%d\n",is_ok);
    iflashDebug("pgm_end_addr:0x%08x\n",MCU_FLASH_PRG_END);
    iflashDebug("total_page:%d\n",total_page);
    iflashDebug("used_page:%0.2f\n",used_page);
    iflashDebug("user_start_page:%d\n",AddrToPage(_start_addr));
    iflashDebug("user_end_page:%d\n",AddrToPage(_end_addr));
    return true;

}

/**
 *@brief    ���캯��,�趨��flash��С = nPage * FLASH_PAGE_SIZE
 *@param    uint8_t startAddr  �ӵڼ���������ʼ
		   uint8_t nPage		ҳ���� > 0
 *@retval   E_FlashStates
*/


/**
  * @brief  Programs a half word at a specified Option Byte Data address.
  * @note   This function can be used for all STM32F10x devices.
  * @param  Address: specifies the address to be programmed.
  * @param  buf: specifies the data to be programmed.
  * @param  iNbrToWrite: the number to write into flash
  * @retval if success return the number to write, -1 if error
  *
  */
#if (FLASH_OVERRIDE == 1)  // ֻ����Ҫд�������,��ǰ�����������ݱ��ֲ���
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
    static uint8_t tmp[2048];
#endif

    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    
    //if ((_start_addr == 0) || (iAddress >= _end_addr))
    {
        iflashDebug("write: flash prg end��0x%x , page size = 0x%x\n", MCU_FLASH_PRG_END,FLASH_PAGE_SIZE);

        iflashDebug("write: flash �������������ʼ��ַ��ҳ���� s��0x%x e:0x%x,i:0x%x\n", _start_addr, _end_addr, iAddress);
       // return 0;
    }
    FLASH_UnlockBank1();

    secpos = iAddress & (~(cpu.flash.page_size - 1 )) ; //������ַ
    secoff = iAddress & (cpu.flash.page_size - 1);  //�������ڵ�ƫ��
    secremain = cpu.flash.page_size - secoff;       //����ʣ��ռ��С
    iflashDebug("secpos = 0x%x, secoff = 0x%x, secremain = 0x%x \n", secpos, secoff, secremain);
    if(iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //������4096���ֽ�

    while( 1 )
    {
        ebox_flashRead(secpos, tmp, cpu.flash.page_size);   //������������
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
            write_without_check(secpos, tmp, cpu.flash.page_size);   //д����������
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
            secpos += cpu.flash.page_size;
            if (secpos >= _end_addr)
            {
                iflashDebug("write: �ռ䲻�㣬����flash��С��\n");
                return -1;
            }
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain;  //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain;  //�ֽ����ݼ�
            if(iNumByteToWrite > cpu.flash.page_size) secremain = cpu.flash.page_size; //��һ����������д����
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
    uint32_t secpos, iAddress;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;

    
    
    int size = _end_addr - _start_addr;
    iAddress = _start_addr + offsetAdd;
    if (offsetAdd >= size || offsetAdd <= 0)
    {
        iflashDebugErr("write: offsetAdd=%d ���������޶���Χ\n",offsetAdd);
        return 0;
    }

    FLASH_UnlockBank1();
    secpos = iAddress & (~(cpu.flash.page_size - 1 )) ; //������ַ(ȡ12-31bitλ)
    secoff = iAddress & (cpu.flash.page_size - 1);  	//�������ڵ�ƫ��(0-11bitλ)
    secremain = cpu.flash.page_size - secoff;       	//����ʣ��ռ��С
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
            secpos += cpu.flash.page_size;
            if (secpos >= _end_addr)
            {
                iflashDebugErr("write: secpos=%d �ռ䲻�㣬����flash��С��\n",secpos);
                return -1;
            }
            secoff = 0;//ƫ��λ��Ϊ0
            buf += secremain;  //ָ��ƫ��
            iAddress += secremain;//д��ַƫ��
            iNumByteToWrite -= secremain;  //�ֽ����ݼ�
            if (iNumByteToWrite > cpu.flash.page_size)
            {
                secremain = cpu.flash.page_size; //��һ����������д����
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
    int size = _end_addr - _start_addr;
    uint32_t iAddress = _start_addr + offsetAdd;
    if (offsetAdd >= size || offsetAdd <= 0)
    {
        iflashDebugErr("write: offsetAdd=%d ���������޶���Χ\n",offsetAdd);
        return 0;
    }
    return ebox_flashRead(iAddress, buf, iNbrToRead);
}

uint32_t Flash::getSize()
{
    return (_end_addr - _start_addr);
}



