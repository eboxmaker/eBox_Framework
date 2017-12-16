#include "ebox.h"
#include "ADS8866.h"




void setup()
{
    ebox_init();
    uart1.begin(115200);
    uart1.println();

    while(1);

}
int main(void)
{
    setup();

    int i = 0 ;
    while(1)
    {

    }
}
