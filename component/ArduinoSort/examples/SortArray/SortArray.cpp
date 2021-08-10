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
#include "ArduinoSort/ArduinoSort.h"

void printArray(String msg, double* myArray) {
  uart1.println(msg);
  uart1.println(myArray[0]);
  uart1.println(myArray[1]);
  uart1.println(myArray[2]);
  uart1.println(myArray[3]);
  uart1.println(myArray[4]);
}

void setup()
{
    ebox_init();
    uart1.begin(115200);
  
    
}
int main(void)
{

    setup();
    while(1)
    {
        // Set up array of 5 numbers
        double myArray[5];
        myArray[0] = 3;
        myArray[1] = 1;
        myArray[2] = 4711;
        myArray[3] = 2;
        myArray[4] = 0;

        // Not sorted
        printArray("Not sorted:", myArray);

        // Sort normally
        sortArray(myArray, 5);
        printArray("sortArray:", myArray);

        // Sort in reverse
        sortArrayReverse(myArray, 5);
        printArray("sortArrayReverse:", myArray);

        // Get the largest number:
        sortArrayReverse(myArray, 5);
        int largest = myArray[0];
        uart1.print("Largest: ");
        uart1.println(largest);

        uart1.println("Done! Sleep and repeat...");
        delay_ms(7000);
    }
}




