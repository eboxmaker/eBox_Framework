/***************************************************
  This is a library for the AM2315 Humidity & Temp Sensor

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

#include "AM2315.h"


Adafruit_AM2315::Adafruit_AM2315(SoftI2c *i2c)
{

    this->i2c = i2c;

}


bool Adafruit_AM2315::begin(uint32_t speed)
{

    this->speed = speed;

    i2c->begin(speed);

    // try to read data, as a test
    return readData();
}

bool Adafruit_AM2315::readData(void)
{
    uint8_t reply[10];

    //  // Wake up the sensor
    //  Wire.beginTransmission(AM2315_I2CADDR);
    //  delay(2);
    //  Wire.endTransmission();

    //  // OK lets ready!
    //  Wire.beginTransmission(AM2315_I2CADDR);
    //  Wire.write(AM2315_READREG);
    //  Wire.write(0x00);  // start at address 0x0
    //  Wire.write(4);  // request 4 bytes data
    //  Wire.endTransmission();
    //
    //  delay(10); // add delay between request and actual read!

    //  Wire.requestFrom(AM2315_I2CADDR, 8);
    //  for (uint8_t i=0; i<8; i++) {
    //    reply[i] = Wire.read();
    //    //Serial.println(reply[i], HEX);
    //  }

    i2c->start();
    i2c->send_7bits_address(AM2315_I2CADDR);
    delay_ms(1);
    i2c->stop();

    i2c->start();
    i2c->send_7bits_address(AM2315_I2CADDR);
    i2c->send_byte(AM2315_READREG);
    i2c->send_byte(0x00);
    i2c->send_byte(4);
    i2c->stop();

    delay_ms(10);

    i2c->read_byte(AM2315_I2CADDR, AM2315_READREG, reply);

    if (reply[0] != AM2315_READREG) return false;
    if (reply[1] != 4) return false; // bytes req'd

    humidity = reply[2];
    humidity *= 256;
    humidity += reply[3];
    humidity /= 10;
    //Serial.print("H"); Serial.println(humidity);

    temp = reply[4] & 0x7F;
    temp *= 256;
    temp += reply[5];
    temp /= 10;
    //Serial.print("T"); Serial.println(temp);

    // change sign
    if (reply[4] >> 7) temp = -temp;

    return true;
}


float Adafruit_AM2315::readTemperature(void)
{
    if (! readData()) return NAN;
    return temp;
}

float Adafruit_AM2315::readHumidity(void)
{
    if (! readData()) return NAN;
    return humidity;
}


/*
 * This method returns both temperature and humidity in a single call and using a single I2C request.
 *
 * If you want to obtain both temperature and humidity when you sample the sensor, be aware that calling
 * readTemperature() and readHumidity() in rapid succession may swamp the sensor and result in invalid
 * readingings (the AM2315 manual advisess that continuous samples must be at least 2 seconds apart).
 * Calling this method avoids the double I2C request.
 */
bool Adafruit_AM2315::readTemperatureAndHumidity(float &t, float &h)
{
    if (!readData()) return false;

    t = temp;
    h = humidity;

    return true;
}

/*********************************************************************/
