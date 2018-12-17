#include "ebox_mem.h"
#include "soft_uart.h"


SoftUart::SoftUart(Gpio *tx_pin, Gpio *rx_pin, uint16_t tx_buffer_size , uint16_t rx_buffer_size )
{
    _tx_pin = tx_pin;
    _rx_pin = rx_pin;
    _tx_buffer_size = tx_buffer_size;
    _rx_buffer_size = rx_buffer_size;
}

void SoftUart::begin(uint32_t baud_rate)
{

    _triger = 96000 / baud_rate;

#ifdef UART_9_BIT
    _tx_ptr = (uint16_t *)ebox_malloc(_tx_buffer_size * sizeof(uint16_t));
    _rx_ptr = (uint16_t *)ebox_malloc(_rx_buffer_size * sizeof(uint16_t));
#else
    _tx_ptr = (uint8_t *)ebox_malloc(_tx_buffer_size * sizeof(uint8_t));
    _rx_ptr = (uint8_t *)ebox_malloc(_rx_buffer_size * sizeof(uint8_t));
#endif

    _tx_pin->mode(OUTPUT_PP_PU);
    _rx_pin->mode(INPUT_PU);
}


int SoftUart::available()
{

}
int SoftUart::peek(void)
{

}
int SoftUart::read()
{

}
int SoftUart::availableForWrite()
{

}
void SoftUart::flush()
{

}
size_t SoftUart::write(uint8_t c)
{
    uint16_t i = (_tx_buffer_head + 1) % _tx_buffer_size;//计算头的位置
    // head = tail, 缓冲区过满，先发送
    while (i == _tx_buffer_tail)
    {
        i = (_tx_buffer_head + 1) % _tx_buffer_size;//计算头的位置//必须静静的等待
    }
    // 加入新数据，移动head
    _tx_ptr[_tx_buffer_head] = c;
    _tx_buffer_head = i;

    _tx_enable = 1; //开启发送
    return 1;
}
//    void tx_bufferx_one(USART_TypeDef *uart, uint8_t index)
//    {
//        if (_tx_buffer_head[index] == _tx_buffer_tail[index])//如果空则直接返回
//        {
//            USART_ITConfig(uart, USART_IT_TXE, DISABLE);// Buffer empty, so disable interrupts
//            return;
//        }
//        unsigned char c = _tx_ptr[index][_tx_buffer_tail[index]];   // 取出字符
//        _tx_buffer_tail[index] = (_tx_buffer_tail[index] + 1) % _tx_buffer_size[index];
//        uart->DR = (c & (uint16_t)0x01FF);
//    }
//        if (_tx_buffer_head == _tx_buffer_tail)//如果空则直接返回
//        {
//            _tx_enable = 0;
//            return;
//        }

//1:start

void SoftUart::loop_230400()
{
    static unsigned char ch;
    static unsigned char counter;
    if(_tx_enable)
    {
        if(counter % _triger == 0)
        {
            switch(state)
            {
                case 0:
                    _tx_pin->reset(); 
                    ch = _tx_ptr[_tx_buffer_tail]; 
                    state++;
                    break;
                case 9:
                    _tx_pin->set();    
                    state = 0;
                    counter = 0;
                    _tx_buffer_tail = (_tx_buffer_tail + 1) % _tx_buffer_size;
                    if (_tx_buffer_head == _tx_buffer_tail)//如果空则直接返回
                        _tx_enable = 0;
                    break;
                default:
                    if((ch&(0x01<<( state -1))) == 0)
                        _tx_pin->reset();
                    else
                        _tx_pin->set();
                    state++;
                    break;
            }
        }
        counter++;
    }
}
