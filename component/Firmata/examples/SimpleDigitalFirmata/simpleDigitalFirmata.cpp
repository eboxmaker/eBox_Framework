/**
  ******************************************************************************
  * @file   : *.cpp
  * @author : shentq
  * @version: V1.2
  * @date   : 2016/08/14

  * @brief   ebox application example .
  *
  * Copyright 2016 shentq. All Rights Reserved.
  ******************************************************************************
 */


#include "ebox.h"
#include "bsp_ebox.h"
#include "ebox_mem.h"
#include "firmata.h"
/**
	*	1	动态内存碎片化测试例程
    *   2   block数量越多说明内存碎片化越严重
	*/
/* 定义例程名和例程发布日期 */
#define EXAMPLE_NAME	"动态内存碎片化测试例程"
#define EXAMPLE_DATE	"2018-08-20"



byte previousPIN[TOTAL_PORTS];  // PIN means PORT for input
byte previousPORT[TOTAL_PORTS];

void outputPort(byte portNumber, byte portValue)
{
    // only send the data when it changes, otherwise you get too many messages!
    if (previousPIN[portNumber] != portValue)
    {
        Firmata.sendDigitalPort(portNumber, portValue);
        previousPIN[portNumber] = portValue;
    }
}

void setPinModeCallback(byte pin, int mode)
{
    if (IS_PIN_DIGITAL(pin))
    {
        pinMode(PIN_TO_DIGITAL(pin), mode);
    }
}

void digitalWriteCallback(byte port, int value)
{
    byte i;
    byte currentPinValue, previousPinValue;

    if (port < TOTAL_PORTS && value != previousPORT[port])
    {
        for (i = 0; i < 8; i++)
        {
            currentPinValue = (byte) value & (1 << i);
            previousPinValue = previousPORT[port] & (1 << i);
            if (currentPinValue != previousPinValue)
            {
                digitalWrite(i + (port * 8), currentPinValue);
            }
        }
        previousPORT[port] = value;
    }
}

void analogWriteCallback(byte pin, int value)
{
    //  switch (pin) {
    //    case 9: servo9.write(value); break;
    //    case 10: servo10.write(value); break;
    //    case 3:
    //    case 5:
    //    case 6:
    //    case 11: // PWM pins
    //      analogWrite(pin, value);
    //      break;
    //  }
    analogWrite(pin, value);

}

void setup()
{
    ebox_init();
    uart1.begin(115200);
    print_log(EXAMPLE_NAME, EXAMPLE_DATE);
    firmata_io_init();

    Firmata.setFirmwareVersion(FIRMATA_FIRMWARE_MAJOR_VERSION, FIRMATA_FIRMWARE_MINOR_VERSION);
    Firmata.attach(DIGITAL_MESSAGE, digitalWriteCallback);
    Firmata.attach(SET_PIN_MODE, setPinModeCallback);
    Firmata.begin(57600);
}
int main(void)
{
    setup();
    while(1)
    {
        byte i;

        for (i = 0; i < TOTAL_PORTS; i++)
        {
            outputPort(i, readPort(i, 0xff));
        }

        while (Firmata.available())
        {
            Firmata.processInput();
        }
    }

}


