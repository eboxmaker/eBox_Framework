#ifndef __EBOX_UART_STREAM_H
#define __EBOX_UART_STREAM_H
#include "ebox_uart.h"
#include "Stream.h"

#define RX_BUFFER_SIZE_UNIT 32
class UartStream : public Stream
{
    Uart *uart;
    char *_rx_buffer;
    uint16_t _rx_buffer_head, _rx_buffer_tail;
    uint16_t uart_rx_buffer_size;

public:
    UartStream(Uart *uart):
        _rx_buffer_head(0),
        _rx_buffer_tail(0)
    {
        this->uart = uart;
        uart_rx_buffer_size = 2 * RX_BUFFER_SIZE_UNIT;
    }
    void begin(uint32_t baud_rate, uint8_t _use_dma = 1);
    void end()
    {
        // clear any received data
        _rx_buffer_head = _rx_buffer_tail;
    };

    virtual int available();
    virtual int read();
    virtual int peek();
    virtual int availableForWrite(void);
    virtual void flush(void);


    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    size_t write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    size_t write(long n)
    {
        return write((uint8_t)n);
    }
    size_t write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    size_t write(int n)
    {
        return write((uint8_t)n);
    }

    using Print::write; // pull in write(str) and write(buf, size) from Print

    void rx_evnet();

};
#endif

