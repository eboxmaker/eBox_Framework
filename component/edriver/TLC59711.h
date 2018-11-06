/***************************************************
  This is a library for our Adafruit 24-channel PWM/LED driver

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/products/1455

  These drivers uses SPI to communicate, 3 pins are required to
  interface: Data, Clock and Latch. The boards are chainable

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef _ADAFRUIT_TLC59711_H
#define _ADAFRUIT_TLC59711_H

#include <ebox_core.h>


class Adafruit_TLC59711
{
public:
    Adafruit_TLC59711(uint8_t n, uint8_t c, uint8_t d);
    Adafruit_TLC59711(uint8_t n);

    bool begin(void);

    void setPWM(uint8_t chan, uint16_t pwm);
    void setLED(uint8_t lednum, uint16_t r, uint16_t g, uint16_t b);
    void write(void);
    void spiwriteMSB(uint32_t d);

private:
    uint16_t *pwmbuffer;

    uint8_t BCr, BCg, BCb;
    int8_t numdrivers, _clk, _dat;

};


#endif
