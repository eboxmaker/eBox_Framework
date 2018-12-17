#ifndef __SOFT_UART_H
#define __SOFT_UART_H

#include "ebox_core.h"
#include "stream.h"

#define UART_MAX_BITS   8

#if (UART_MAX_BITS > 8)
#define UART_9_BIT
#endif


class SoftUart: public Stream
{
    
public:
    SoftUart(Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size = 64, uint16_t rx_buffer_size = 64);
    void    begin(uint32_t baud_rate);
    void    end();
    virtual int     available();
    virtual int     peek(void);
    virtual int     read();
    virtual int     availableForWrite();
    virtual void    flush();
    virtual size_t  write(uint8_t c);
    inline  size_t  write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline  size_t  write(long n)
    {
        return write((uint8_t)n);
    }
    inline  size_t  write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline  size_t  write(int n)
    {
        return write((uint8_t)n);
    }
    //    virtual size_t  write(const uint8_t *buffer, size_t size);
    using       Print::write;

    
    void loop_230400();
private:
    Gpio *_tx_pin;
    Gpio *_rx_pin;


    uint8_t *_tx_ptr;          // 缓冲区指针
    uint8_t *_rx_ptr;          // 缓冲区指针

    uint16_t _tx_buffer_size;   // 发送环形缓冲区size
    uint16_t _tx_buffer_head;   // 缓冲区头,每写入（写入缓冲区）一个字符，向后移动1
    uint16_t _tx_buffer_tail;   // 缓冲区尾,每写出（写入串口TX）一个字符，向后移动1

    uint16_t _rx_buffer_size;
    uint16_t _rx_buffer_head;
    uint16_t _rx_buffer_tail;

    uint8_t _tx_enable;
    uint8_t _triger;
    uint8_t state;

};

#endif
