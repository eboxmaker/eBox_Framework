/*
file   : iflash.cpp
author : shentq
version: V1.1
date   : 2015/7/5

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#include "ebox_iflash.h"






uint32_t Flash::read_word(uint32_t Address)
{
    return *(__IO uint32_t *)Address;
}
uint8_t Flash::read_byte(uint32_t Address)
{
    return *(__IO uint32_t *)Address;
}

int Flash::read(uint32_t Address, uint8_t *buf, int32_t num_to_read)
{
    int i = 0;
    while(i < num_to_read )
    {
        *(buf + i) = *(__IO uint8_t *) Address++;
        i += 1;
    }
    return i;

}
/*
 *  ReadAddr  ：起始地址 (uint32_t)address 是 4 的整数倍, (uint32_t)data
 *  pBuffer   ：数据指针
 *  NumToRead ：字（4位）数
 */
void Flash::read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead)
{
    uint32_t i;
    for(i = 0; i < NumToRead; i++)
    {
        pBuffer[i] = read_word(ReadAddr); //读取4个字节
        ReadAddr += 4; //偏移4个字节
    }
}


/*
 *  WriteAddr： 起始地址 (uint32_t)address 是 4 的整数倍, (uint32_t)data
 *  pBuffer：   数据指针
 *  NumToWrite：字（uint32_t）数
 */
void Flash::write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
{
    FLASH_Status status = FLASH_COMPLETE;
    uint32_t addrx = 0;
    uint32_t endaddr = 0;

    if(WriteAddr % 4)return;  //非法地址

    FLASH_Unlock();//解锁
    FLASH_DataCacheCmd(DISABLE);//FLASH擦除期间，必须禁止数据缓存

    addrx = WriteAddr;              //写入起始地址
    endaddr = WriteAddr + NumToWrite * 4; //写入结束地址
    if(addrx < 0X1FFF0000)          //是否为主存储区
    {
        while(addrx < endaddr)      //对非0XFFFF FFFF 的地方，先擦除
        {
            if(read_word(addrx) != 0XFFFFFFFF) //对非0XFFFF FFFF 的地方，先擦除
            {
                status = FLASH_EraseSector(addr_to_sector(addrx), VoltageRange_3); //VCC=2.7~3.6V之间！！
                if(status != FLASH_COMPLETE)break;  //异常
            }
            else addrx += 4;
        }
    }
    if(status == FLASH_COMPLETE)
    {
        while(WriteAddr < endaddr) //写数据
        {
            if(FLASH_ProgramWord(WriteAddr, *pBuffer) != FLASH_COMPLETE) //写入数据
            {
                break;  //写入异常
            }
            WriteAddr += 4;
            pBuffer++;
        }
    }
    FLASH_DataCacheCmd(ENABLE);//FLASH擦除结束，开启数据缓存
    FLASH_Lock();//上锁
    //    Printf("Flash status :%d\r\n",status);
}


int Flash::erase_sector(FLASH_SECTOR_ADDR_t start_addr)
{
    FLASH_Unlock();
    /* Clear pending flags (if any) */
    FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR |
                    FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR | FLASH_FLAG_PGSERR);


    if (FLASH_EraseSector(addr_to_sector(start_addr), VoltageRange_3) != FLASH_COMPLETE)
    {
        /* Error occurred while sector erase.
           User can add here some code to deal with this error  */
        while (1)
        {
            return -1;
        }
    }
    FLASH_Lock();
    return 0;
}



uint32_t Flash::sector_first_addr(uint16_t sector)
{
    switch(sector)
    {
    case FLASH_Sector_0:
        return ADDR_FLASH_SECTOR_0;
    case FLASH_Sector_1:
        return ADDR_FLASH_SECTOR_1;
    case FLASH_Sector_2:
        return ADDR_FLASH_SECTOR_2;
    case FLASH_Sector_3:
        return ADDR_FLASH_SECTOR_3;
    case FLASH_Sector_4:
        return ADDR_FLASH_SECTOR_4;
    case FLASH_Sector_5:
        return ADDR_FLASH_SECTOR_5;
    case FLASH_Sector_6:
        return ADDR_FLASH_SECTOR_6;
    case FLASH_Sector_7:
        return ADDR_FLASH_SECTOR_7;
    case FLASH_Sector_8:
        return ADDR_FLASH_SECTOR_8;
    case FLASH_Sector_9:
        return ADDR_FLASH_SECTOR_9;
    case FLASH_Sector_10:
        return ADDR_FLASH_SECTOR_10;
    case FLASH_Sector_11:
        return ADDR_FLASH_SECTOR_11;
    }
    return 0;
}

uint32_t Flash::sector_size(uint16_t sector)
{
    if(sector < FLASH_Sector_4)
    {
        return 0x4000;
    }
    else if((sector < FLASH_Sector_5) && (sector >= FLASH_Sector_4))
    {
        return 0x10000;
    }
    else if((sector < FLASH_Sector_12) && (sector >= FLASH_Sector_5))
    {
        return 0x20000;
    }
    return 0;
}

uint16_t Flash::addr_to_sector(uint32_t Address)
{

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        return FLASH_Sector_0;
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        return FLASH_Sector_1;

    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        return FLASH_Sector_2;
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        return FLASH_Sector_3;
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        return FLASH_Sector_4;
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        return FLASH_Sector_5;
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        return FLASH_Sector_6;
    }
    else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        return FLASH_Sector_7;
    }
    else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        return FLASH_Sector_8;
    }
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        return FLASH_Sector_9;
    }
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        return FLASH_Sector_10;
    }
    return 0;
}

