/**
  ******************************************************************************
  * @file    bsp.cpp
  * @author  shentq
  * @version V2.0
  * @date    2016/10/20
  * @brief   
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form 
  * or means, without the prior written consent of shentq. This specification is 
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2016 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */
  /*
	1.֧��ebox���е�SPI�ӿ�
	2.֧�ֵ������߹��ض��FLASH
	3.֧�ֶ������߹��ض��ٸ�FLASH
*/

/**
 * Modification History:
 * -shentq                  -version 1.0(2016/10/20)
 *  *ɾ��ȫ�ֻ��������������ö�̬�����ڴ�
 */

#include "stdio.h"
#include "stdlib.h"
#include <stdarg.h>

#include "ebox.h"
#include <sfud.h>

/**********************Ӳ��SPI�ӿ�*******************************************/
struct flash_port
{
    uint8_t             index;
    Spi                 *spi ;
    Gpio                *cs;
    SPI_CONFIG_TYPE     config;
};//flash �ײ�ӿ�

//�û���������
struct flash_port flash_port_table[]=
{    
    /*��һ��FLASHӲ����������*/
    {
        0               ,/*FLASH������*/
        &spi1          ,/*�����ӵ�SPI*/
        &PA15           ,/*CS����*/
        1               ,/*spi����ΨһID�����������ϴ�ID���Բ����ظ���ȡֵ��Χ[0-255]*/
        SPI_MODE0       ,/*FLASH��֧�ֵ�SPIģʽ0��1��2��3*/
        SPI_CLOCK_DIV2  ,/*SPIʱ�ӷ�Ƶ*/
        SPI_BITODER_MSB ,/*��λ��ǰ���ǵ�λ��ǰ��MSB�����λ��ǰ*/
    }
};
/***************************************************************************/


/**********************���SPI�ӿ�*******************************************/
//struct flash_port
//{
//    uint8_t             index;
//    SoftSpi             *spi ;
//    Gpio                *cs;
//    SPI_CONFIG_TYPE     config;
//};//flash �ײ�ӿ�

////�û���������
//struct flash_port flash_port_table[]=
//{    
//    /*��һ��FLASHӲ����������*/
//    {
//        0               ,/*FLASH������*/
//        &sspi1          ,/*�����ӵ�SPI*/
//        &PA15           ,/*CS����*/
//        1               ,/*spi����ΨһID�����������ϴ�ID���Բ����ظ���ȡֵ��Χ[0-255]*/
//        SPI_MODE0       ,/*FLASH��֧�ֵ�SPIģʽ0��1��2��3*/
//        SPI_CLOCK_DIV2  ,/*SPIʱ�ӷ�Ƶ*/
//        SPI_BITODER_MSB ,/*��λ��ǰ���ǵ�λ��ǰ��MSB�����λ��ǰ*/
//    }
//};
/***************************************************************************/
char log_buf[256];

static sfud_err     spi_write_read(const sfud_spi *spi, const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,size_t read_size);
static void         spi_lock(const sfud_spi *spi) ;
static void         spi_unlock(const sfud_spi *spi) ;
static void         spi_delay_100us();

extern "C"{

/*
    FLASH��Ӧ��spi���г�ʼ��
*/    
sfud_err sfud_spi_port_init(sfud_flash *flash) 
{
    
    sfud_err result = SFUD_SUCCESS;
    
    flash->spi.wr = spi_write_read;
    flash->spi.lock = spi_lock;
    flash->spi.unlock = spi_unlock;
    flash->spi.user_data = &flash->index;//��flash��index���ݸ�sfud_spi��userdata�����ڵײ����flash��Ӧ��spi��������
    /* about 100 microsecond delay */
    flash->retry.delay = spi_delay_100us;
    /* adout 60 seconds timeout */
    flash->retry.times = 60 * 10000;

    uint8_t *_index  =(uint8_t*) flash->spi.user_data;//����flash��index
    uint8_t index  = *_index;
    
    flash_port_table[index].spi->begin(&flash_port_table[index].config);
    flash_port_table[index].cs->mode(OUTPUT_PP);
    flash_port_table[index].cs->set();

    return result;
}
}
/*
    spi�Ķ�д����
    SPI�����sfud_spi *sf_spi�еĲ���user_data�仯���Զ����л�spi��cs���Լ�spi��������Ϣ��
    ����ʵ�ֵ������߹��ض��FLASH�����߶������߹��ض��FLASH��
    һ�����ߴ���â��״̬���κδ������ϵ��豸ֻ�ܵȴ���ǽ������������������ʹ��Ȩ����ܽ��С�
*/    
static sfud_err spi_write_read(const sfud_spi *sf_spi,const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,size_t read_size)
{
    uint8_t     *_index  =(uint8_t*) sf_spi->user_data;//����flash��index
    uint8_t     index  = *_index;
    sfud_err    result  = SFUD_SUCCESS;
    uint8_t     send_data, read_data;

    flash_port_table[index].spi->take_spi_right(&flash_port_table[index].config);
    flash_port_table[index].cs->reset();
    for (size_t i = 0, retry_times; i < write_size + read_size; i++) 
    {
        if (i < write_size) 
        {
            send_data = *write_buf++;
        } 
        else 
        {
            send_data = 0xff;
        }
        read_data = flash_port_table[index].spi->transfer(send_data);
        if (i >= write_size) 
        {
            *read_buf++ = read_data;
        }
    }
    flash_port_table[index].cs->set();
    flash_port_table[index].spi->release_spi_right();
    return result;
}

/*
    ��ֹ�����л�
*/
static void spi_lock(const sfud_spi  *spi) 
{
    no_interrupts();
}

/*
    ���������л�
*/
static void spi_unlock(const sfud_spi  *spi) 
{
    interrupts();
}


/*
    100us��ʱ
*/
static void spi_delay_100us()
{
    delay_us(100);
}










extern "C"{

void ebox_printf(const char *fmt , ... )
{
    char *buf;

    size_t size2=128;
    int ret;

    va_list va_params;
    va_start(va_params, fmt);
    
    do{
        buf = (char *)ebox_malloc(size2);
        if(buf == NULL)
            return ;
        ret = _vsnprintf(buf, size2,fmt, va_params);
        if((ret == -1) || (ret > size2))
        {
            size2+=128;
            ebox_free(buf);
            ret = -1;
        }
    }while(ret == -1);
    va_end(va_params);
    uart1.printf_length((const char*)buf, ret);
    //uart1.printf(" ====%d===", ebox_get_free());
    //uart1.wait_busy();
    ebox_free(buf);
}
/**
 * This function is print debug info.
 *
 * @param file the file which has call this function
 * @param line the line number which has call this function
 * @param format output format
 * @param ... args
 */
void sfud_log_debug(const char *file, const long line, const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    ebox_printf("[SFUD](%s:%ld) ", file, line);
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    ebox_printf("%s\n", log_buf);
    va_end(args);
}

/**
 * This function is print routine info.
 *
 * @param format output format
 * @param ... args
 */
void sfud_log_info(const char *format, ...) {
    va_list args;

    /* args point to the first variable parameter */
    va_start(args, format);
    ebox_printf("[SFUD]");
    /* must use vprintf to print */
    vsnprintf(log_buf, sizeof(log_buf), format, args);
    ebox_printf("%s\n", log_buf);
    va_end(args);
}
}