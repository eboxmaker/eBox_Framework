/*
  Modbus-Arduino Example - Lamp (Modbus Serial)
  Copyright by Andr¨¦ Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <Modbus.h>
#include <ModbusSerial.h>
#include "bsp_ebox.h"
// Modbus Registers Offsets (0-9999)
const int LAMP1_HOLD = 0;
const int LAMP2_HOLD = 1;

Pwm pwmled1(&LED1);
Pwm pwmled2(&LED2);

ModbusSerial mb;

void setup()
{
    ebox_init();
    mb.config(&uart1, 115200, &PA5);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);


    pwmled1.begin(1000, 500);
    pwmled2.begin(1000, 500);

    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addHreg(LAMP1_HOLD);
    mb.addHreg(LAMP2_HOLD);

}
int main()
{
    setup();
    while(1)
    {

        // Call once inside loop() - all magic here
        mb.task();
        // Attach ledPin to LAMP1_COIL register
        pwmled1.set_duty(mb.Hreg(LAMP1_HOLD));
        pwmled2.set_duty(mb.Hreg(LAMP2_HOLD));

    }

}