#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#include "Map.h"

int main() {
    Map testMap;
    
    // Initiate array to send in.
    char* benny = new char[25];
    
    benny[0] = 19;
    benny[1] = 'M';
    benny[2] = 0;
    
    for (int i = 3; i < 32; i++) {
        benny[i] = 'u';
    }
    
    // insert into map
    
    for (int i = 0; i < 32; i++) {
        benny[2] = i;
        testMap.setColAsChar(benny);
    }
    
    testMap.printMap();
    
    
    return 0;
}