/**
  ******************************************************************************
  * @file    iflash.h
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

/* Define to prevent recursive inclusion -------------------------------------*/

#ifndef   __IFLASH_H__
#define   __IFLASH_H__

#include "ebox_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "mcu.h"


class Flash
{
public:

    Flash();

    /**
    *@brief    初始化,设定的flash大小 = nPage * cpu.flash.page_size
                选中的page从后向前数nPage个页，用于用户空间
       uint8_t nPage		页面数 > 0
    *@retval   false 失败，true 成功
    */
    bool begin(int nPage = 0xFF);
    /**
    *@brief    读出一组数据
    *@param    uint32_t offsetAdd  	要读取的地址
       uint8_t *buf			保存读取的数据
       uint32_t iNbrToWrite	要读取的数据长度
    *@retval   读出的数据长度。 如果 0 错误
    */
    int read(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToRead) ;
    /**
    *@brief    写入一组数据
    *@param    uint32_t offsetAdd  	要写入的地址
       uint8_t *buf			要写入的数据
       uint32_t iNbrToWrite	要写入的数据长度
    *@retval   写入的数据长度。 如果 0 错误
    */
    int write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite);
    uint32_t getSize();

private:
    uint32_t _start_addr;	// 起始地址
    uint32_t _end_addr;		// 结束地址
};


#ifdef __cplusplus
}
#endif

#endif
