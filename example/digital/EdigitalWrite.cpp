
#include "ebox.h"

void setup()
{
	eBoxInit();
	PA7->mode(OUTPUT_PP);
}
int main(void)
{
	setup();
	while(1)
	{
		PA7->set();
		PA7->reset();
	}

}




