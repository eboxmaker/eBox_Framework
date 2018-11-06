#ifndef __FIRMATA_IO_H
#define __FIRMATA_IO_H
#include  "ebox.h"

extern Adc adc1;
#define Serial  uart1
#define Serial1 Serial

#define delay(x) delay_ms(x)

#ifdef __cplusplus
extern "C" {
#endif
void firmata_io_init();
void pinMode(uint8_t, uint8_t);
void digitalWrite(uint8_t, uint8_t);
int digitalRead(uint8_t);
int analogRead(uint8_t);
void analogReference(uint8_t mode);
void analogWrite(uint8_t, int);

#ifdef __cplusplus
}
#endif

#endif
