#ifndef _ebox_core_h
#define _ebox_core_h
/**
  ******************************************************************************
  * @file    core.h
  * @author  shentq
  * @version V2.1
  * @date    2016/08/14
  * @brief   此文件为ebox核心文件,提供了诸多非常重要的接口,可以被标准的c文件调用。
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

#include "ebox_type.h"
#include "binary.h"
#include "ebox_config.h"


#define EBOX_VERSION "2.3.1"

#define EBOX_MAJOR_VERSION           2 // 
#define EBOX_MINOR_VERSION           3 // 
#define EBOX_BUGFIX_VERSION          1 // 

#if __cplusplus
extern "C" {
#endif

    extern Cpu_t cpu;

    extern void        (*interrupts)(void);
    extern int         (*no_interrupts)(void);
    extern void        (*ebox_reset)();
    extern uint32_t    (*micros)();
    extern uint32_t    (*millis)();
    extern void        (*delay_ms)(uint32_t ms);
    extern void        (*delay_us)(uint32_t us);
    extern size_t       ebox_printf(const char *fmt, ...);
    extern int          ebox_uart_putc(int ch);
    extern size_t       ebox_uart_write(const char *buffer, size_t size);



    // 取最大值，最小值
#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v2)))
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))
#define min2v(v1, v2 )        ((v1)<(v2)? (v1):(v2))
#define max2v(v1, v2 )        ((v1)>(v2)? (v1):(v2))
    //#define abs(x)              ((x)>0?(x):-(x))
#define constrain(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define round(x)            ((x)>=0?(long)((x)+0.5):(long)((x)-0.5))
#define roundl(x)            ((x)>=0?(long)((x)-0.5):(long)((x)+0.5))
#define radians(deg)        ((deg)*DEG_TO_RAD)
#define degrees(rad)        ((rad)*RAD_TO_DEG)
#define sq(x)               ((x)*(x))


#define low_byte(w) ((uint8_t) ((w) & 0xff))
#define high_byte(w) ((uint8_t) ((w) >> 8))

    /** read bit of value */
#define bit_read(value, bit) (((value) >> (bit)) & 0x01)

    /** set bit to 1 of value */
#define bit_set(value, bit) ((value) |= (1UL << (bit)))

    /** set bit to 0 of value */
#define bit_clear(value, bit) ((value) &= ~(1UL << (bit)))

    /** write bit of value*/
#define bit_write(value, bit, bitvalue) (bitvalue ? bit_set(value, bit) : bit_clear(value, bit))

    /** Bits m to n of x */
#define bits_get(x, m, n) ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))

    /** 1UL shifted left by 'shift' */
#define bit_shift(shift)                     (1UL << (shift))

    /** 'Mask' shifted left by 'shift' */
#define bit_mask_shift(mask, shift)    ((mask) << (shift))

    /** True iff v is a power of two (1, 2, 4, 8, ...) */
#define is_pow_tow(v)  ((v) && !((v) & ((v) - 1)))



#define lowByte(w) ((uint8_t)           ((w) & 0xff))
#define highByte(w) ((uint8_t)          ((w) >> 8))

#define bitRead(value, bit)             (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)              ((value) |= (1UL << (bit)))
#define bitClear(value, bit)            ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue)  (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitsGet(x, m, n)                ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))
#define bitShift(shift)                 (1UL << (shift))
#define bitMaskShift(mask, shift)       ((mask) << (shift))
#define isPow2(v)                       ((v) && !((v) & ((v) - 1)))
#define	getArraySize(__ARR)									(sizeof(__ARR)/sizeof(__ARR[0]))		// 获取数组大小


void        random_seed(unsigned int seed);
unsigned int    random(unsigned int min, unsigned int max);
char        char2digital(char c);
uint8_t     bcd_to_dec(uint8_t bcd_code);
uint8_t     dec_to_bcd(uint8_t dec); 

#if __cplusplus
} // extern "C"
#endif


#ifdef __cplusplus
#include "ebox_retarget.h"
#include "object.h"
#include "WCharacter.h"
#include "WString.h"
#include "port/ebox_port_gpio.h"
#include "port/ebox_port_spi.h"
#include "port/ebox_port_i2c.h"


uint16_t makeWord(unsigned int w);
uint16_t makeWord(unsigned char h, unsigned char l);
#define word(...) makeWord(__VA_ARGS__)


void            shift_out(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order, uint8_t val);
uint8_t         shift_in(Gpio *data_pin, Gpio *clock_pin, uint8_t bit_order);

unsigned int    random();
unsigned int    random(unsigned int max);
long            map(long, long, long, long, long);


//限制某个数的下界
template<typename T>
void limitLow(T &num, T limL)
{
    if (num < limL)
    {
        num = limL;
    }
}

//限制某个数的上界
template<typename T>
void limitHigh(T &num, T limH)
{
    if (num > limH)
    {
        num = limH;
    }
}


//限制某个数的上下界
template<typename T>
void limit(T &num, T limL, T limH)
{
    limitLow(num, limL);
    limitHigh(num, limH);
}


template <class T>
void swap(T *a, T *b)
{
    T temp;
    temp = *a;
    *a = *b;
    *b = temp;
}
#endif
#endif

