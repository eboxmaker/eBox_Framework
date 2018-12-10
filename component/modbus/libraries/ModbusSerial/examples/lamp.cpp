/*
  Modbus-Arduino Example - Lamp (Modbus Serial)
  Copyright by Andr¨¦ Sarmento Barbosa
  http://github.com/andresarmento/modbus-arduino
*/

#include <Modbus.h>
#include <ModbusSerial.h>
#include "bsp_ebox.h"
#include "ebox_mem.h"
// Modbus Registers Offsets (0-9999)
const int LAMP1_COIL = 0;
const int LAMP2_COIL = 1;
const int LAMP3_COIL = 2;

ModbusSerial mb;

void setup()
{
    ebox_init();
    mb.config(&UART, 115200);
    // Set the Slave ID (1-247)
    mb.setSlaveId(10);

    LED1.mode(OUTPUT_PP);
    LED2.mode(OUTPUT_PP);
    LED1.write(0);
    delay_ms(1000);
    LED1.write(1);
    delay_ms(1000);
    // Add LAMP1_COIL register - Use addCoil() for digital outputs
    mb.addCoil(LAMP1_COIL);
    mb.addCoil(LAMP2_COIL);
    mb.addCoil(LAMP3_COIL);
    mb.Coil(LAMP2_COIL, 1);
    mb.Coil(LAMP1_COIL, 1);
}
int main()
{
    setup();
    while(1)
    {
        // Call once inside loop() - all magic here
        mb.task();
        // Attach ledPin to LAMP1_COIL register
        LED1.write( mb.Coil(LAMP1_COIL));
        LED2.write( mb.Coil(LAMP2_COIL));
        UART.printf("free mem  \r\n");
    }

}