#ifndef __EBOX_CFUN_H
#define __EBOX_CFUN_H

#ifdef __cplusplus
 extern "C" {
#endif
     
#include "math.h"
     
#include "../core/stdFun/big_little.h"
#include "../core/stdFun/crc.h"
#include "../core/stdFun/ebox_mem.h"
#include "../core/stdFun/ebox_printf.h"
#include "../core/stdFun/fifo.h"
#include "../core/stdFun/itoa.h"
#include "../core/stdFun/math_misc.h"
#include "../core/stdFun/util.h"
 
extern Cpu_t cpu;
     
extern void        (*interrupts)(void);
extern int         (*no_interrupts)(void);
extern void        (*ebox_reset)();
extern uint64_t    (*micros)();
extern uint64_t    (*millis)();
extern void        (*delay_ms)(uint64_t ms);
extern void        (*delay_us)(uint64_t ms);


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
#define is_power_of_tow(v)  ((v) && !((v) & ((v) - 1)))

#ifdef __cplusplus
}
#endif

#endif
