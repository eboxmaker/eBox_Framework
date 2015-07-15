
#include "ebox.h"

void setup()
{
	eBoxInit();
	
	PA7->mode(OUTPUT_PP);
//	pinMode(PA7,OUTPUT_PP);
}

int16_t x;
int main(void)
{
	setup();
	while(1)
	{
		PA7->set();
		PA7->reset();
//		digitalWrite(PA7,1);
//		digitalWrite(PA7,0);
}


}




