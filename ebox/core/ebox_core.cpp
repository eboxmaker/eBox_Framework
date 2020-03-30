#include "ebox_core.h"


#if __cplusplus
extern "C" {
#endif
    void        (*interrupts)(void);
    int         (*no_interrupts)(void);
    void        (*ebox_reset)(void);
    uint32_t    (*micros)(void);
    uint32_t    (*millis)(void);
    void        (*delay_ms)(uint32_t ms);
    void        (*delay_us)(uint32_t us);

    Cpu_t cpu;
#if __cplusplus
} // extern "C"
#endif

uint16_t makeWord(unsigned int w)
{
    return w;
}
uint16_t makeWord(unsigned char h, unsigned char l)
{
    return (h << 8) | l;
}

char char2digital(
    char c	/**< is a character('0'-'F') to convert to HEX */
)
{
    if (c >= '0' && c <= '9')
        return c - '0';
    if (c >= 'a' && c <= 'f')
        return 10 + c - 'a';
    if (c >= 'A' && c <= 'F')
        return 10 + c - 'A';

    return (char)c;
}

uint8_t bcd_to_dec(uint8_t bcd_code)
{
    uint8_t temp, dec;
    temp = (bcd_code >> 4) * 10;
    dec = (bcd_code & 0x0f) + temp;
    return dec;
}
uint8_t dec_to_bcd(uint8_t dec)
{
    uint8_t temp, temp2, bcd;
    temp = dec / 10;
    temp2 = dec % 10;

    bcd =  (temp << 4) + temp2;

    return bcd;
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

/**
 * @brief
 *
 * @param
 *
 *
 *
 * @retval  NONE
 */
void random_seed(unsigned int seed)
{
    srand(seed);
}

/**
 * @brief   产生一个0~65535的随机数
 * @param   NONE
 * @retval  0~65535随机数结果
 */
unsigned int random()
{
    return rand();
}
/**
 * @brief   产生一个小于某个值得随机数
 * @param   max:随机数的最大值，此值比产生的最大值大1.
 * @retval  随机数结果
 */
unsigned int random(unsigned int max)
{
    return (rand() % max);
}

/**
 * @name
 * @brief
 *
 * @param
 *
 *
 *
 * @retval  NONE
 */
unsigned int random(unsigned int min, unsigned int max)
{
    return (min + rand() % (max - min));
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
