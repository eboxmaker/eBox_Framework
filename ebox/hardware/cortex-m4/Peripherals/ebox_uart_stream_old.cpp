#include "ebox_mem.h"
#include "ebox_uart_stream.h"


void UartStream::begin(uint32_t baud_rate, uint8_t _use_dma /*= 1*/)
{
    _rx_buffer = (char *)ebox_malloc(uart_rx_buffer_size);
    uart->begin(baud_rate, _use_dma);
    uart->attach(this, &UartStream::rx_evnet, RxIrq);
    uart->interrupt(RxIrq, ENABLE);
}
int UartStream::available()
{
    return ((unsigned int)(uart_rx_buffer_size + _rx_buffer_head - _rx_buffer_tail)) % uart_rx_buffer_size;
};
int UartStream::peek()
{
    if (_rx_buffer_head == _rx_buffer_tail)
    {
        return -1;
    }
    else
    {
        return _rx_buffer[_rx_buffer_tail];
    }
}
int UartStream::read()
{
    // if the head isn't ahead of the tail, we don't have any characters
    if (_rx_buffer_head == _rx_buffer_tail)
    {
        return -1;
    }
    else
    {
        unsigned char c = _rx_buffer[_rx_buffer_tail];
        _rx_buffer_tail = (uint8_t)(_rx_buffer_tail + 1) % uart_rx_buffer_size;
        return c;
    }
}
int UartStream::availableForWrite(void)
{
    return 0;
}
void UartStream::flush(void)
{

}

size_t UartStream::write(uint8_t c)
{
    return uart->write(c);
}
size_t UartStream::write(const uint8_t *buffer, size_t size)
{
    return uart->write(buffer, size);
}
void UartStream::rx_evnet()
{

    // No Parity error, read byte and store it in the buffer if there is
    // room
    unsigned char c = uart->read();
    uint8_t i = (unsigned int)(_rx_buffer_head + 1) % uart_rx_buffer_size;

    // if we should be storing the received character into the location
    // just before the tail (meaning that the head would advance to the
    // current location of the tail), we're about to overflow the buffer
    // and so we don't write the character or advance the head.
    if (i != _rx_buffer_tail)
    {
        _rx_buffer[_rx_buffer_head] = c;
        _rx_buffer_head = i;
    }
    else  //环形缓冲区溢出，需要扩容
    {
        no_interrupts();
        char *temp = (char *)ebox_malloc(uart_rx_buffer_size);
        ebox_memcpy(temp, _rx_buffer, uart_rx_buffer_size);
        ebox_free(_rx_buffer);

        uart_rx_buffer_size += RX_BUFFER_SIZE_UNIT;
        _rx_buffer = (char *)ebox_malloc(uart_rx_buffer_size);
        ebox_memcpy(_rx_buffer, temp, uart_rx_buffer_size);
        ebox_free(temp);

        i = (unsigned int)(_rx_buffer_head + 1) % uart_rx_buffer_size;
        _rx_buffer[_rx_buffer_head] = c;
        _rx_buffer_head = i;

        interrupts();
        // Parity error, read byte but discard it
    };
}
