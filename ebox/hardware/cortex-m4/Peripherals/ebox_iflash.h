/*
file   : iflash.h
author : shentq
version: V1.1
date   : 2016/4/6

Copyright 2015 shentq. All Rights Reserved.

Copyright Notice
No part of this software may be used for any commercial activities by any form or means, without the prior written consent of shentq.

Disclaimer
This specification is preliminary and is subject to change at any time without notice. shentq assumes no responsibility for any errors contained herein.
*/

#ifndef   __EBOX_IFLASH_H
#define   __EBOX_IFLASH_H



#include "ebox_common.h"



typedef enum{
/* Base address of the Flash sectors */ 
 ADDR_FLASH_SECTOR_0   =  ((uint32_t)0x08000000), /* Base address of Sector 0, 16 Kbytes   */
 ADDR_FLASH_SECTOR_1   =  ((uint32_t)0x08004000), /* Base address of Sector 1, 16 Kbytes   */
 ADDR_FLASH_SECTOR_2   =  ((uint32_t)0x08008000), /* Base address of Sector 2, 16 Kbytes   */
 ADDR_FLASH_SECTOR_3   =  ((uint32_t)0x0800C000), /* Base address of Sector 3, 16 Kbytes   */
 ADDR_FLASH_SECTOR_4   =  ((uint32_t)0x08010000), /* Base address of Sector 4, 64 Kbytes   */
 ADDR_FLASH_SECTOR_5   =  ((uint32_t)0x08020000), /* Base address of Sector 5, 128 Kbytes  */
 ADDR_FLASH_SECTOR_6   =  ((uint32_t)0x08040000), /* Base address of Sector 6, 128 Kbytes  */
 ADDR_FLASH_SECTOR_7   =  ((uint32_t)0x08060000), /* Base address of Sector 7, 128 Kbytes  */
 ADDR_FLASH_SECTOR_8   =  ((uint32_t)0x08080000), /* Base address of Sector 8, 128 Kbytes  */
 ADDR_FLASH_SECTOR_9   =  ((uint32_t)0x080A0000), /* Base address of Sector 9, 128 Kbytes  */
 ADDR_FLASH_SECTOR_10  =  ((uint32_t)0x080C0000), /* Base address of Sector 10, 128 Kbytes */
 ADDR_FLASH_SECTOR_11  =  ((uint32_t)0x080E0000), /* Base address of Sector 11, 128 Kbytes */

#if defined (USE_STM324x7I_EVAL) || defined (USE_STM324x9I_EVAL) 
 ADDR_FLASH_SECTOR_12   =  ((uint32_t)0x08100000), /* Base address of Sector 12, 16 Kbytes  */
 ADDR_FLASH_SECTOR_13   =  ((uint32_t)0x08104000), /* Base address of Sector 13, 16 Kbytes  */
 ADDR_FLASH_SECTOR_14   =  ((uint32_t)0x08108000), /* Base address of Sector 14, 16 Kbytes  */
 ADDR_FLASH_SECTOR_15   =  ((uint32_t)0x0810C000), /* Base address of Sector 15, 16 Kbytes  */
 ADDR_FLASH_SECTOR_16  =   ((uint32_t)0x08110000), /* Base address of Sector 16, 64 Kbytes  */
 ADDR_FLASH_SECTOR_17  =   ((uint32_t)0x08120000), /* Base address of Sector 17, 128 Kbytes */
 ADDR_FLASH_SECTOR_18  =   ((uint32_t)0x08140000), /* Base address of Sector 18, 128 Kbytes */
 ADDR_FLASH_SECTOR_19  =   ((uint32_t)0x08160000), /* Base address of Sector 19, 128 Kbytes */
 ADDR_FLASH_SECTOR_20  =   ((uint32_t)0x08180000), /* Base address of Sector 20, 128 Kbytes */
 ADDR_FLASH_SECTOR_21  =   ((uint32_t)0x081A0000), /* Base address of Sector 21, 128 Kbytes */
 ADDR_FLASH_SECTOR_22  =   ((uint32_t)0x081C0000), /* Base address of Sector 22, 128 Kbytes */
 ADDR_FLASH_SECTOR_23  =   ((uint32_t)0x081E0000), /* Base address of Sector 23, 128 Kbytes */
#endif /* USE_STM324x7I_EVAL or USE_STM324x9I_EVAL */
}FLASH_SECTOR_ADDR_t;

class FLASHCLASS
{
public:


    int erase_sector(FLASH_SECTOR_ADDR_t start_addr);
    int read(uint32_t start_addr, uint8_t *buf, int32_t num_to_read) ;
    int write_sector(FLASH_SECTOR_ADDR_t start_addr, uint8_t *buf, uint32_t num_to_write);
private:
    uint16_t write_without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite);
    uint16_t addr_to_sector(uint32_t Address);

};


#endif
