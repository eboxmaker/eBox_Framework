
#include "ebox.h"
#include "lcd1602.h"
using namespace std;
u32 mill_time;
u32 sec_time;



#define  R_XS 7
#define  Y_XS 5
#define  G_XS 7


#define R_Time 7000
#define Y_Time 5000
#define G_Time 7000




enum LED_STATE
{ R_flag=0,
	Y_flag=1,
	G_flag=2
}; 

LED_STATE flag;

LCD1602 lcd(&PB12,&PB13,&PB14,&PB15,&PB0,&PB1,&PB2,&PB3,&PB4,&PB5,&PB6,&PB7);


void setup()
{
		ebox_init();
		uart1.begin(9600);
		PB8.mode(OUTPUT_PP);
		PB9.mode(OUTPUT_PP);
		PB10.mode(OUTPUT_PP);
	  PB8.set();
	  PB9.reset();
	  PB10.reset();
	  flag=R_flag;
		mill_time=millis();
		sec_time=millis();
		lcd.begin();
}
		                                                                                                                        


	
	
int main(void)
{
	setup();
	while(1)
	{
	
		
		if(flag==R_flag)
		{
			if( (millis()-sec_time)>1000)
		{
			sec_time=millis();
 
						
			lcd.printf(2,0,"test=%02d",R_XS-((millis()-mill_time)/1000));
			
		}
			if( millis()- mill_time > R_Time  )
			{
				mill_time=millis();
				flag=Y_flag;
				PB9.set();
				PB8.reset();
		
				
			}
		}
		
		
		
				if(flag==Y_flag)
		{
			
			
			if( (millis()-sec_time)>1000)
		{
			sec_time=millis();
 
						
			lcd.printf(2,0,"test=%02d",Y_XS-((millis()-mill_time)/1000));
			
		}
			if( millis()-mill_time >Y_Time )
			{
				mill_time=millis();
				flag=G_flag;
				PB10.set();
				PB9.reset();			
	
				
			}
		}
		
		
		
				if(flag==G_flag)
					
		{
			
			if( (millis()-sec_time)>1000)
		{
			sec_time=millis();
 
						
				lcd.printf(2,0,"test=%02d",G_XS-((millis()-mill_time)/1000));
			
		}
			if( millis()-mill_time > G_Time )
			{
				mill_time=millis();
				flag=R_flag;
				PB8.set();
				PB10.reset();
				
				
			}
		}
		
		
			
		
		
		
	
	
	}
}

/* //???
void setup()
{
		ebox_init();
		uart1.begin(9600);
		PB8.mode(OUTPUT_PP);
		PB9.mode(OUTPUT_PP);
		PB10.mode(OUTPUT_PP);
		PB8.set();
		PB10.reset();
		delay_ms(2000);	
		PB8.reset();
		PB9.set();
		delay_ms(2000);
	  PB9.reset();
		PB10.set();
	  delay_ms(2000);
}

*/


