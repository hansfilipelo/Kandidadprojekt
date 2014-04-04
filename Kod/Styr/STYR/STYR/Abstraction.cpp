#ifndef _Abstraction_cpp
#define _Abstraction_cpp

/* 
	File with abstractions for use in AVR project. Mainly related to improving the use of Arrays since we can't use Vectors or Strings.
	Created by: hansfilipelo
*/


// -----------------------------
// Usage: 
//		ArrayPointer = pushBackChar(inArray, char);

char* pushBackChar(char* inArray, char inChar){
    
    // Get size of Array
    int size = sizeof(inArray) / sizeof(inArray[0]);
    
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
#endif
