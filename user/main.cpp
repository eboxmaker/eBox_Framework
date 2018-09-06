#include "ebox.h"
#include "bsp_ebox.h"

#include "EventGpio.h"
#include "eventmanager.h"




#define EXAMPLE_NAME	"fast rfft  example"
#define EXAMPLE_DATE	"2018-08-28"

#include "ads1271.h"

ADS1271 ad(&spi1);
    int32_t adc_value;

void event()
{
    PB8.toggle();
    PB8.toggle();
    adc_value = ad.read();
//    uart1.printf("%0.9f\r\n",2.51*adc_value/8388608.0);

//    PB8.toggle();
}
EventGpio event_io_2(&PA0,0,0,0,event,0,0,0);
EventManager io_manager;

void setup()
{
        
	ebox_init();

    UART.begin(115200);
    print_log(EXAMPLE_NAME,EXAMPLE_DATE);

    ad.begin();
    event_io_2.begin(0);
    io_manager.add(&event_io_2);
    PB8.mode(OUTPUT);
    no_interrupts();
}

int main(void)
{

    
	setup();
    while (1)
	{
        io_manager.process();
	}
    
}