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
    *@brief    ��ʼ��,�趨��flash��С = nPage * cpu.flash.page_size
                ѡ�е�page�Ӻ���ǰ��nPage��ҳ�������û��ռ�
       uint8_t nPage		ҳ���� > 0
    *@retval   false ʧ�ܣ�true �ɹ�
    */
    bool begin(int nPage = 0xFF);
    /**
    *@brief    ����һ������
    *@param    uint32_t offsetAdd  	Ҫ��ȡ�ĵ�ַ
       uint8_t *buf			�����ȡ������
       uint32_t iNbrToWrite	Ҫ��ȡ�����ݳ���
    *@retval   ���������ݳ��ȡ� ��� 0 ����
    */
    int read(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToRead) ;
    /**
    *@brief    д��һ������
    *@param    uint32_t offsetAdd  	Ҫд��ĵ�ַ
       uint8_t *buf			Ҫд�������
       uint32_t iNbrToWrite	Ҫд������ݳ���
    *@retval   д������ݳ��ȡ� ��� 0 ����
    */
    int write(uint32_t offsetAdd, uint8_t *buf, uint32_t iNbrToWrite);
    uint32_t getSize();

private:
    uint32_t _start_addr;	// ��ʼ��ַ
    uint32_t _end_addr;		// ������ַ
};


#ifdef __cplusplus
}
#endif

#endif
