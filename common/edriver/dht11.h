/*
 * Dht11.h
 *
 * Version 0.6
 *
 * DHT11 Temperature and humidity sensor library for Arduino.
 *
 * See http://playground.arduino.cc/main/DHT11Lib
 *
 * License:   GPL v3 (http://www.gnu.org/licenses/gpl.html)
 * Datasheet: http://www.micro4you.com/files/sensor/DHT11.pdf
 *
 * Modification History:
 *   - George Hadjikyriacou                     - Version 0.1 (??)
 *   - SimKard                                  - Version 0.2 (24/11/2010)
 *   - Rob Tillaart                             - Version 0.3 (28/03/2011)
 *       * Added comments
 *       * Removed all non-DHT11 specific code
 *       * Added References
 *   - Rob Tillaart                             - Version 0.4 (17/03/2012)
 *       * Added Arduino 1.0 support
 *   - Rob Tillaart                             - Version 0.4.1 (19/05/2012)
 *       * Added error codes
 *   - Andy Dalton                              - Version 0.5 (13/09/2013)
 *       * Replaced magic numbers with named constants
 *       * Factored out repeated code into a helper method.
 *       * Made pin a parameter to the constructor instead of a parameter
 *         to read()
 *       * Changed the error codes to an enumeration
 *       * Update the temp/humidity only if the checksum is correct
 *       * Added more comments
 *    - Link                            			  - Version 0.6 (2016/01/03)
 *       * port from ardunio to eBox
 */

#ifndef dht11_h
#define dht11_h

#include "ebox.h"

/*
 * Dht11
 *
 * A class that modes a DHT11 humidity/temperature sensor.
 */
class Dht11
{
private:
    // The last read humidity value
    int humidity;

    // The last read temperature value
    int temperature;

    // The pin over which we communicate with the sensor
    Gpio *pin;


public:
    // The version of this library
    static const char *const VERSION;

    // An enumeration modeling the read status of the sensor.
    enum ReadStatus
    {
        OK,
        ERROR_CHECKSUM,
        ERROR_TIMEOUT,
    };

    /*
     * Dht11
     *
     * Constructs a new Dht11 object that communicates with a DHT11 sensor
     * over the given pin.
     */
    Dht11(Gpio *p_pin)
    {
        pin = p_pin;
    }

    /*
     * read
     *
     * Update the humidity and temperature of this object from the sensor.
     * Returns OK if the update was successful, ERROR_TIMEOUT if it times out
     * waiting for a response from the sensor, or ERROR_CHECKSUM if the
     * calculated checksum doesn't match the checksum provided by the sensor.
     */
    ReadStatus read();

    /*
     * getHumidity
     *
     * Gets the last read relative humidity percentage.
     */
    inline int getHumidity() const
    {
        return this->humidity;
    }

    /*
     * getTemperature
     *
     * Gets the last read temperature value in degrees Celsius.
     */
    inline int getTemperature() const
    {
        return this->temperature;
    }

private:
    enum
    {
        /*
         * Default value for the maximum number of iterations performed by
         * the waitForPinChange function.
         */
        MAX_PIN_CHANGE_ITERATIONS = 10000,
    };


    /*
     * waitForPinChange
     *
     * Wait for the the data pin on the sensor to change from the given oldValue
     * to the opposite value (i.e., from HIGH -> LOW or from LOW -> HIGH).  The
     * function performs a tight loop decrementing the given maxIterations.  If
     * the state of the pin changes before maxIterations reaches 0, the function
     * returns OK; otherwise it returns ERROR_TIMEOUT.
     *
     * This is a private method used only by the Dht11 class.
     */
    inline ReadStatus waitForPinChange(const int oldValue,
                                       unsigned  maxIterations =
                                           MAX_PIN_CHANGE_ITERATIONS) const
    {
        while ((--maxIterations > 0) && (pin->read() == oldValue))
        {
        }

        return (maxIterations > 0) ? OK : ERROR_TIMEOUT;
    }
};

#endif
