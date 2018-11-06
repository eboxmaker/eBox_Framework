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


#include <TLC59711.h>

Adafruit_TLC59711::Adafruit_TLC59711(uint8_t n, uint8_t c, uint8_t d)
{
    numdrivers = n;
    _clk = c;
    _dat = d;

    BCr = BCg = BCb = 0x7F;

    pwmbuffer = (uint16_t *)calloc(2, 12 * n);
}

Adafruit_TLC59711::Adafruit_TLC59711(uint8_t n)
{
    numdrivers = n;
    _clk = -1;
    _dat = -1;

    //  SPI.setBitOrder(MSBFIRST);
    //#ifdef __arm__
    //  SPI.setClockDivider(42);
    //#else
    //  SPI.setClockDivider(SPI_CLOCK_DIV8);
    //#endif
    //  SPI.setDataMode(SPI_MODE0);
    //  BCr = BCg = BCb = 0x7F;

    //  pwmbuffer = (uint16_t *)calloc(2, 12*n);
}

void  Adafruit_TLC59711::spiwriteMSB(uint32_t d)
{

    //  if (_clk >= 0) {
    //    uint32_t b = 0x80;
    //    //  b <<= (bits-1);
    //    for (; b!=0; b>>=1) {
    //      digitalWrite(_clk, LOW);
    //      if (d & b)
    //	digitalWrite(_dat, HIGH);
    //      else
    //	digitalWrite(_dat, LOW);
    //      digitalWrite(_clk, HIGH);
    //    }
    //  } else {
    //    SPI.transfer(d);
    //  }
}

void Adafruit_TLC59711::write(void)
{
    //  uint32_t command;

    //  // Magic word for write
    //  command = 0x25;

    //  command <<= 5;
    //  //OUTTMG = 1, EXTGCK = 0, TMGRST = 1, DSPRPT = 1, BLANK = 0 -> 0x16
    //  command |= 0x16;

    //  command <<= 7;
    //  command |= BCr;

    //  command <<= 7;
    //  command |= BCg;

    //  command <<= 7;
    //  command |= BCb;

    //  noInterrupts();
    //  for (uint8_t n=0; n<numdrivers; n++) {
    //    spiwriteMSB(command >> 24);
    //    spiwriteMSB(command >> 16);
    //    spiwriteMSB(command >> 8);
    //    spiwriteMSB(command);

    //    // 12 channels per TLC59711
    //    for (int8_t c=11; c >= 0 ; c--) {
    //      // 16 bits per channel, send MSB first
    //      spiwriteMSB(pwmbuffer[n*12+c]>>8);
    //      spiwriteMSB(pwmbuffer[n*12+c]);
    //    }
    //  }

    //  if (_clk >= 0)
    //    delayMicroseconds(200);
    //  else
    //    delayMicroseconds(2);
    //  interrupts();
}



void Adafruit_TLC59711::setPWM(uint8_t chan, uint16_t pwm)
{
    //  if (chan > 12*numdrivers) return;
    //  pwmbuffer[chan] = pwm;
}


void Adafruit_TLC59711::setLED(uint8_t lednum, uint16_t r, uint16_t g, uint16_t b)
{
    //  setPWM(lednum*3, r);
    //  setPWM(lednum*3+1, g);
    //  setPWM(lednum*3+2, b);
}


bool Adafruit_TLC59711::begin()
{
    //  if (!pwmbuffer) return false;

    //  if (_clk >= 0) {
    //    pinMode(_clk, OUTPUT);
    //    pinMode(_dat, OUTPUT);
    //  } else {
    //    SPI.begin();
    //  }
    return true;
}
