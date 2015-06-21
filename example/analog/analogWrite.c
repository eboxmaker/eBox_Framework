
#include "math.h"
#include "arduino.h"


//PWM P(7);
/*
analog pin table
0,1,2,3,6,7,16,17,22,23,24,25,26,27
*/
void setup()
{
	systemtickcfg(9000);//1ms??????
}

float x,y;
uint32_t i;
int main(void)
{

	setup();
	
	
	
	while(1)
	{
		if(i < 1000)
		 x ++;
		else
			i=0;
		y = fabsf(x);
		analogWrite(7,500);

	}


}




