
char* pushBackChar(char* inArray, char inChar){
    
    // Get size of Array
    int size = sizeof(inArray) / sizeof(inArray[0]);
    
    // Create a bigger array
    char* outArray = new char[size + 1];
    
    // Add char in the beginning and then add the rest
    outArray[0] = inChar;
    
    for (int i = 0; i < size; i++) {
        outArray[i+1] = inArray[i];
    }
    
    return outArray;
}