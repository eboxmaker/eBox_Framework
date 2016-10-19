#ifndef __EBOX_SPI_PORT_H
#define __EBOX_SPI_PORT_H
#ifdef __cplusplus
extern "C" {
#endif
 
#include "stm32f10x.h"
#include "stdlib.h"

void spi_init(uint8_t dev_num);
void spi_translate(const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,size_t read_size);
int spi_write(const uint8_t *write_buf, size_t write_size);
int spi_read(uint8_t *read_buf,size_t read_size);
void spi_lock(void  *spi);
void spi_unlock(void  *spi);
void spi_delay_100us();
void ebox_printf(char *fmt , ... );
    
    
    
#ifdef __cplusplus
}
#endif
#endif