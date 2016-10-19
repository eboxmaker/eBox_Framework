#include "ebox.h"
#include "ebox_spi_port.h"
#include "stdio.h"
#include "stdlib.h"

SPI_CONFIG_TYPE     spi_dev_sfud;
Spi *sfud_spi =  &spi1;
Gpio *sfud_cs = &PA15;

void spi_init(uint8_t dev_num)
{
    spi_dev_sfud.dev_num = dev_num;
    spi_dev_sfud.mode = SPI_MODE0;
    spi_dev_sfud.prescaler = SPI_CLOCK_DIV4;
    spi_dev_sfud.bit_order = SPI_BITODER_MSB;
    
    sfud_spi->begin(&spi_dev_sfud);
    sfud_cs->mode(OUTPUT_PP);
    sfud_cs->set();
}
void spi_translate(const uint8_t *write_buf, size_t write_size, uint8_t *read_buf,size_t read_size)
{
    uint8_t send_data, read_data;
    sfud_spi->take_spi_right(&spi_dev_sfud);
    sfud_cs->reset();
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
        read_data = sfud_spi->transfer(send_data);
        if (i >= write_size) 
        {
            *read_buf++ = read_data;
        }
    }
    sfud_cs->set();
    sfud_spi->release_spi_right();
}
int spi_write(const uint8_t *write_buf, size_t write_size)
{
    sfud_spi->take_spi_right(&spi_dev_sfud);
    sfud_cs->reset();
    sfud_spi->write((uint8_t *)write_buf,write_size);
    sfud_cs->set();
    sfud_spi->release_spi_right();
    return 0;
}

int spi_read(uint8_t *read_buf,size_t read_size)
{
    sfud_spi->take_spi_right(&spi_dev_sfud);
    sfud_cs->reset();
    sfud_spi->read((uint8_t *)read_buf,read_size);
    sfud_cs->set();
    sfud_spi->release_spi_right();
    return 0;
}

void spi_lock(void  *spi) 
{
    //no_interrupts();
}

void spi_unlock(void  *spi) 
{
    //interrupts();
}
void spi_delay_100us()
{
    delay_us(100);
}

void ebox_printf(char *fmt , ... )
{
    char *buf;

    size_t size1=128,size2=128;

    va_list va_params;
    va_start(va_params, fmt);
    
    do{
        buf = (char *)ebox_malloc(size2);
        size1 = _vsnprintf(buf, size2,fmt, va_params);
        if(size1 == -1)
            size2+=128;
        ebox_free(buf);
    }while(size1 == -1);

    buf = (char *)ebox_malloc(size1);
    vsprintf(buf, fmt, va_params); //存在内存溢出的风险
    va_end(va_params);
    uart1.printf_length((const char*)buf, size1);
    uart1.wait_busy();
    ebox_free(buf);
}
