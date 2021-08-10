#include <ArduinoSort.h>

void setup() {
  Serial.begin(9600);
  delay(3000);
}

void loop() {

  // Set up array of 5 numbers
  int myArray[5];
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
  Serial.print("Largest: ");
  Serial.println(largest);

  Serial.println("Done! Sleep and repeat...");
  delay(7000);
}

void printArray(String msg, int* myArray) {
  Serial.println(msg);
  Serial.println(myArray[0]);
  Serial.println(myArray[1]);
  Serial.println(myArray[2]);
  Serial.println(myArray[3]);
  Serial.println(myArray[4]);
}
