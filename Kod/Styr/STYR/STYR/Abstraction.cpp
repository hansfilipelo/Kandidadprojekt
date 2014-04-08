/* 
	File with abstractions for use in AVR project. Mainly related to improving the use of Arrays since we can't use Vectors or Strings.
	Created by: hansfilipelo
*/

#include "Abstraction.h"

using namespace std;

// -----------------------------
// Usage: 
//		ArrayPointer = pushBackChar(inArray, char);

void pushBackChar(char* inArray, char inChar){
    
    // Get size of Array
    int size = strlen(inArray);
    //Går att skriva mindre generellt just för vårt fall, då vet vi hur stor      //    arrayen är (Tobias)
    
    for (int i = size; i > 0; i--) {
        inArray[i] = inArray[i-1];
    }
    
    inArray[0] = inChar;
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

// -----------

// Takes char array and takes char by char and then converts to int

int charToInt(char* inArray){
	
	 // Get size of Array
	 //int size = sizeof(inArray) / sizeof(inArray[0]);
    int size = strlen(inArray);
    
    int powerNr = size - 1;
	
	int output = 0;
	
	for (int i = 0; powerNr >= 0; i++){
        output = output + (inArray[i] * pow(10,powerNr));
        
        powerNr = powerNr - 1;
	 }
    
    return output;
}

// -------------------

int getValueFromArray(char* inArray, int valueNr){
    char* outArray = new char[3];
    
    int start = valueNr*3;
    for (int i = start; i < start + 3; i++) {
        outArray[i - start] = inArray[i];
    }
    
    return charToInt(outArray);
}






