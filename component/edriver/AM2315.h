/***************************************************
  This is a library for the AM2315 Humidity Pressure & Temp Sensor

  Designed specifically to work with the AM2315 sensor from Adafruit
  ----> https://www.adafruit.com/products/1293

  These displays use I2C to communicate, 2 pins are required to
  interface
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#include "ebox.h"

#define AM2315_I2CADDR       0x5C
#define AM2315_READREG       0x03

class Adafruit_AM2315
{
public:
    Adafruit_AM2315(SoftI2c *i2c);
    bool begin(uint32_t speed);
    float readTemperature(void);
    float readHumidity(void);
    bool readTemperatureAndHumidity(float &, float &);

private:
    SoftI2c *i2c;
    uint32_t speed;
    bool readData(void);
    float humidity, temp;
};

