#ifndef ArduinoSort_h
#define ArduinoSort_h

/**** These are the functions you can use ****/

// Sort an array
template<typename AnyType> void sortArray(AnyType array[], size_t sizeOfArray);

// Sort in reverse
template<typename AnyType> void sortArrayReverse(AnyType array[], size_t sizeOfArray);

// Sort an array with custom comparison function
template<typename AnyType> void sortArray(AnyType array[], size_t sizeOfArray, bool (*largerThan)(AnyType, AnyType));

// Sort in reverse with custom comparison function
template<typename AnyType> void sortArrayReverse(AnyType array[], size_t sizeOfArray, bool (*largerThan)(AnyType, AnyType));





/**** Implementation below. Do not use below functions ****/

namespace ArduinoSort {
	template<typename AnyType> bool builtinLargerThan(AnyType first, AnyType second) {
		return first > second;
	}

	template<> bool builtinLargerThan(char* first, char* second) {
		return strcmp(first, second) > 0;
	}

	template<typename AnyType> void insertionSort(AnyType array[], size_t sizeOfArray, bool reverse, bool (*largerThan)(AnyType, AnyType)) {
		for (size_t i = 1; i < sizeOfArray; i++) {
			for (size_t j = i; j > 0 && (largerThan(array[j-1], array[j]) != reverse); j--) {
				AnyType tmp = array[j-1];
				array[j-1] = array[j];
				array[j] = tmp;
			}
		}
	}
}

template<typename AnyType> void sortArray(AnyType array[], size_t sizeOfArray) {
	ArduinoSort::insertionSort(array, sizeOfArray, false, ArduinoSort::builtinLargerThan);
}

template<typename AnyType> void sortArrayReverse(AnyType array[], size_t sizeOfArray) {
	ArduinoSort::insertionSort(array, sizeOfArray, true, ArduinoSort::builtinLargerThan);
}

template<typename AnyType> void sortArray(AnyType array[], size_t sizeOfArray, bool (*largerThan)(AnyType, AnyType)) {
	ArduinoSort::insertionSort(array, sizeOfArray, false, largerThan);
}

template<typename AnyType> void sortArrayReverse(AnyType array[], size_t sizeOfArray, bool (*largerThan)(AnyType, AnyType)) {
	ArduinoSort::insertionSort(array, sizeOfArray, true, largerThan);
}


#endif
