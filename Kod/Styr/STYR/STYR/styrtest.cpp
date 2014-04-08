#ifdef __APPLE__
#define DEBUG 1
#endif

#include <stdio.h>
#include <iostream>
#include "MapSection.h"
#include "Map.h"
#include "Abstraction.h"

using namespace std;

int main(){
    
    //Initiate
    Map* mom = new Map();
    Robot* testis = new Robot(16,0,mom);
    
    // Test code
    cout << "--------" << " Printing map " << "--------" << endl;
    mom->printMap();
    cout << "----------" << endl;
    cout << endl;
    cout << "--------" << " Testing intToChar " << "--------" <<  endl;
    char* testChar = intToChar(1234);
    for (int i = 0; i < 4; i++) {
        cout << (int)testChar[i] << " ";
    }
    cout << endl;
    cout << "Size of Array: " << sizeof(testChar) / sizeof(testChar[0]) << endl;
    cout << "Items in array: " << strlen(testChar) << endl;
    cout << "----------" << endl;
    cout << endl;
    cout << "Testing XX" << endl;
    
    
    
    
    delete testis;
    delete mom;
    
	return 0;
}
