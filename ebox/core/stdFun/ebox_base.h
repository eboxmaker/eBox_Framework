#ifndef _ebox_base_h
#define _ebox_base_h
#include "ebox_type.h"

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



#define lowByte(w) ((uint8_t)           ((w) & 0xff))
#define highByte(w) ((uint8_t)          ((w) >> 8))
#define bitRead(value, bit)             (((value) >> (bit)) & 0x01)
#define bitSet(value, bit)              ((value) |= (1UL << (bit)))
#define bitClear(value, bit)            ((value) &= ~(1UL << (bit)))
#define bitWrite(value, bit, bitvalue)  (bitvalue ? bitSet(value, bit) : bitClear(value, bit))
#define bitsGet(x, m, n)                ((((uint32)x) << (31 - (n))) >> ((31 - (n)) + (m)))
#define bitShift(shift)                 (1UL << (shift))
#define bitMaskShift(mask, shift)       ((mask) << (shift))

#endif

