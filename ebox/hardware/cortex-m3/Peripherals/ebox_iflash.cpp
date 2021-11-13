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
// 是否打印调试信息, 1打印,0不打印
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
        iflashDebugErr("没有足够的空间page_remained=%d\n",page_remained);
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
 *@brief    构造函数,设定的flash大小 = nPage * FLASH_PAGE_SIZE
 *@param    uint8_t startAddr  从第几个扇区开始
		   uint8_t nPage		页面数 > 0
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
#if (FLASH_OVERRIDE == 1)  // 只更新要写入的数据,当前区块其他数据保持不变
int Flash::write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite)
{
    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos, iAddress = _start_addr + offsetAdd;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;
    uint16_t i = 0;
    // 定义缓冲区，保存读出的页数据
#if USE_EBOX_MEM
    uint8_t *tmp = (uint8_t *)ebox_malloc(FLASH_PAGE_SIZE);
#else
    static uint8_t tmp[2048];
#endif

    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    
    //if ((_start_addr == 0) || (iAddress >= _end_addr))
    {
        iflashDebug("write: flash prg end：0x%x , page size = 0x%x\n", MCU_FLASH_PRG_END,FLASH_PAGE_SIZE);

        iflashDebug("write: flash 定义错误，请检查起始地址和页数量 s：0x%x e:0x%x,i:0x%x\n", _start_addr, _end_addr, iAddress);
       // return 0;
    }
    FLASH_UnlockBank1();

    secpos = iAddress & (~(cpu.flash.page_size - 1 )) ; //扇区地址
    secoff = iAddress & (cpu.flash.page_size - 1);  //在扇区内的偏移
    secremain = cpu.flash.page_size - secoff;       //扇区剩余空间大小
    iflashDebug("secpos = 0x%x, secoff = 0x%x, secremain = 0x%x \n", secpos, secoff, secremain);
    if(iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //不大于4096个字节

    while( 1 )
    {
        ebox_flashRead(secpos, tmp, cpu.flash.page_size);   //读出整个扇区
        for(i = 0; i < secremain; i++)   //校验数据
        {
            if(tmp[secoff + i] != 0XFF)break;   //需要擦除
        }
        if(i < secremain)  //需要擦除
        {
            FLASHStatus = FLASH_ErasePage(secpos); //擦除这个扇区
            if(FLASHStatus != FLASH_COMPLETE)
                return -1;
            for(i = 0; i < secremain; i++) //复制
            {
                tmp[i + secoff] = buf[i];
            }
            write_without_check(secpos, tmp, cpu.flash.page_size);   //写入整个扇区
        }
        else
        {
            write_without_check(iAddress, buf, secremain); //写已经擦除了的,直接写入扇区剩余区间.
        }

        if(iNumByteToWrite == secremain)  //写入结束了
        {
            break;
        }
        else     //不够写入，移到下一个扇区
        {
            secpos += cpu.flash.page_size;
            if (secpos >= _end_addr)
            {
                iflashDebug("write: 空间不足，请检查flash大小！\n");
                return -1;
            }
            secoff = 0;//偏移位置为0
            buf += secremain;  //指针偏移
            iAddress += secremain;//写地址偏移
            iNumByteToWrite -= secremain;  //字节数递减
            if(iNumByteToWrite > cpu.flash.page_size) secremain = cpu.flash.page_size; //下一个扇区还是写不完
            else secremain = iNumByteToWrite;  //下一个扇区可以写完了
        }

    }
#if USE_EBOX_MEM
    ebox_free(tmp);
#endif
    FLASH_LockBank1();
    return iNbrToWrite;
}
#else
// 擦除原有数据，写入新数据
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
        iflashDebugErr("write: offsetAdd=%d 参数超过限定范围\n",offsetAdd);
        return 0;
    }

    FLASH_UnlockBank1();
    secpos = iAddress & (~(cpu.flash.page_size - 1 )) ; //扇区地址(取12-31bit位)
    secoff = iAddress & (cpu.flash.page_size - 1);  	//在扇区内的偏移(0-11bit位)
    secremain = cpu.flash.page_size - secoff;       	//扇区剩余空间大小
    // 当前扇区剩余空间可以写完数据
    if (iNumByteToWrite <= secremain) secremain = iNumByteToWrite; //不大于4096个字节

    while ( 1 )
    {
        FLASH_ErasePage(secpos); //擦除这个扇区
        write_without_check(iAddress, buf, secremain); //写已经擦除了的,直接写入扇区剩余区间.

        // 当前扇区剩余空间够写入
        if (iNumByteToWrite == secremain) //写入结束了
            break;
        else	//不够写入，移到下一个扇区
        {
            secpos += cpu.flash.page_size;
            if (secpos >= _end_addr)
            {
                iflashDebugErr("write: secpos=%d 空间不足，请检查flash大小！\n",secpos);
                return -1;
            }
            secoff = 0;//偏移位置为0
            buf += secremain;  //指针偏移
            iAddress += secremain;//写地址偏移
            iNumByteToWrite -= secremain;  //字节数递减
            if (iNumByteToWrite > cpu.flash.page_size)
            {
                secremain = cpu.flash.page_size; //下一个扇区还是写不完
            }
            else
            {
                secremain = iNumByteToWrite;  //下一个扇区可以写完了
            }
        }
    }
    FLASH_LockBank1();
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
    int size = _end_addr - _start_addr;
    uint32_t iAddress = _start_addr + offsetAdd;
    if (offsetAdd >= size || offsetAdd <= 0)
    {
        iflashDebugErr("write: offsetAdd=%d 参数超过限定范围\n",offsetAdd);
        return 0;
    }
    return ebox_flashRead(iAddress, buf, iNbrToRead);
}

uint32_t Flash::getSize()
{
    return (_end_addr - _start_addr);
}



