#include "ebox_core.h"

 extern "C" {

void        (*interrupts)(void);
int         (*no_interrupts)(void);
void        (*ebox_reset)();
uint64_t    (*micros)();
uint64_t    (*millis)();
void        (*delay_ms)(uint64_t ms);
void        (*delay_us)(uint64_t ms);

 
Cpu_t cpu;
}

/** @defgroup common函数
  * @{
  */
/**
 *@brief    Gpio按照时钟连续输出一个u8类型的数据，用于串行通信。
 *@param    data_pin：  数据输入引脚
            clock_pin:  时钟信号的引脚
            bit_order:  数据大小端控制LSB_FIRST：低位先发送；MSB_FIRST高位先发送
 *@retval   接收到的数据
*/
uint8_t shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
        clock_pin->write(HIGH);
        if (bit_order == LSB_FIRST)
            value |= data_pin->read() << i;
        else
            value |= data_pin->read() << (7 - i);
        clock_pin->write(LOW);
    }
    return value;
}


/**
 * @brief   Gpio按照时钟连续输出一个u8类型的数据，用于串行通信。
 * @param   data_pin：  数据输出引脚
 *          clock_pin:  时钟信号的引脚
 *          bit_order:  数据大小端控制LSB_FIRST：低位先发送；MSB_FIRST高位先发送
 *          val：       要发送的数据
 * @retval  NONE
 */
void shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val)
{
    int i;
    for (i = 0; i < 8; i++)
    {
        if (bit_order == LSB_FIRST)
            data_pin->write(!!(val & (1 << i)));
        else
            data_pin->write(!!(val & (1 << (7 - i))));

        clock_pin->write(HIGH);
        clock_pin->write(LOW);

    }
}


