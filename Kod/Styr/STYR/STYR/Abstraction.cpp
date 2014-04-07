/* 
	File with abstractions for use in AVR project. Mainly related to improving the use of Arrays since we can't use Vectors or Strings.
	Created by: hansfilipelo
*/

#include "Abstraction.h"

using namespace std;

// -----------------------------
// Usage: 
//		ArrayPointer = pushBackChar(inArray, char);

char* pushBackChar(char* inArray, char inChar){
    
    // Get size of Array
    int size = sizeof(inArray) / sizeof(inArray[0]);
    //Går att skriva mindre generellt just för vårt fall, då vet vi hur stor      //    arrayen är (Tobias)
    
    // Create a bigger array
    char* outArray = new char[size + 1];
    
    // Add char in the beginning and then add the rest
    outArray[0] = inChar;
    
    for (int i = 0; i <= size; i++) {
        outArray[i+1] = inArray[i];
    }
    
	delete inArray;
    return outArray;
}

// ---------------------------

int* pushBackInt(int* inArray, int inChar){
    
    // Get size of Array
    int size = sizeof(inArray) / sizeof(inArray[0]);
    
    // Create a bigger array
    int* outArray = new int[size + 1];
    
    // Add int in the beginning and then add the rest
    outArray[0] = inChar;
    
    for (int i = 0; i <= size; i++) {
        outArray[i+1] = inArray[i];
    }
    
	delete inArray;
    return outArray;
}

// -----------

// Takes int and takes number by number and then converts to char

char* intToChar(int inInt){
	char* output;
	int temp = inInt;
	int nrOfDigits = 0;
	
	// Get number of digits in int
	while (pow(10,nrOfDigits) <= inInt){
		nrOfDigits++;
	}
	nrOfDigits = nrOfDigits - 1;
	
	// Create char array as output
	output = new char[nrOfDigits];
	int temp2;
	int i = 0;
	
	// Get number as char
	while (temp > 0){
		temp2 = temp / pow(10,nrOfDigits);
		
		output[i] = temp2;
		
		temp = temp - (pow(10,nrOfDigits) * temp2);
		i++;
		nrOfDigits = nrOfDigits - 1;
	}
	
	return output;
}