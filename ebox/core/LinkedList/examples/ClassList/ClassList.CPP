/**
  ******************************************************************************
  * @file    DateTiemTest.cpp
  * @author  shentq
  * @version V2.0
  * @date    2021/07/23
  * @brief   ebox application example .
  ******************************************************************************
  * @attention
  *
  * No part of this software may be used for any commercial activities by any form
  * or means, without the prior written consent of shentq. This specification is
  * preliminary and is subject to change at any time without notice. shentq assumes
  * no responsibility for any errors contained herein.
  * <h2><center>&copy; Copyright 2015 shentq. All Rights Reserved.</center></h2>
  ******************************************************************************
  */


/* Includes ------------------------------------------------------------------*/


#include "ebox.h"
#include "LinkedList/LinkedList.h"
class Animal {
	public:
		char *name;
		bool isMammal;
};

char  catname[]="kitty";
char  dogname[]="doggie";
char  emuname[]="emu";
LinkedList<Animal> myAnimalList = LinkedList<Animal>();

void setup()
{
    ebox_init();
    uart1.begin(115200);
  
    // Create a Cat
	Animal cat;
	cat.name = catname;
	cat.isMammal = true;

	// Create a dog
	Animal dog;
	dog.name = dogname;
	dog.isMammal = true;

	// Create a emu
	Animal emu ;
	emu.name = emuname;
	emu.isMammal = false; // just an example; no offense to pig lovers

    
	// Add animals to list
	myAnimalList.add(cat);
	myAnimalList.add(emu);
	myAnimalList.add(dog);
    uart1.print("There are ");
	uart1.print(myAnimalList.size());
	uart1.print(" animals in the list. The mammals are: ");

	int current = 0;
	Animal animal;
	for(int i = 0; i < myAnimalList.size(); i++){

		// Get animal from list
		animal = myAnimalList.get(i);

		// If its a mammal, then print it's name
		if(animal.isMammal){

			// Avoid printing spacer on the first element
			if(current++)
				uart1.print(", ");

			// Print animal name
			uart1.print(animal.name);
		}
	}
	uart1.println(".");
}
int main(void)
{

    setup();
    while(1)
    {

        delay_ms(1000);
    }
}




