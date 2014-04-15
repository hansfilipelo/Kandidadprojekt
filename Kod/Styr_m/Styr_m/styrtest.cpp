#ifdef __APPLE__
#define DEBUG 1
#endif

#include <stdio.h>
#include <iostream>
#include "MapSection.h"
#include "Map.h"
#include "Abstraction.h"
#include "Communication.h"
#include "slave.h"

using namespace std;

int main(){
    
    //Initiate
    Map* mom = new Map();
    Slave* testSlave = new Slave();
    Communication* testComm = new Communication(testSlave);
    Robot* testRobot = new Robot(16,1,mom, testComm);
    testComm->setRobot(testRobot);
    
    // Map testing
    // -----------------------------
    cout << "--------" << " Printing map " << "--------" << endl;
    mom->printMap();
    cout << endl;
    
    mom->getPos(16,15)->setType('c');
    mom->printMap();
    
    cout << "----------" << endl;
    cout << endl;
    
    // Testing to get column as char array for communications
    // -----------------------------
    cout << "--------" << " getColAsChar" << "--------" << endl;
    for (int i = 0; i < 25; i++) {
        cout << mom->getColAsChar(16)[i] << endl;
    }
    cout << "----------" << endl;
    cout << endl;
    
    // Abstraction function testing
    // -----------------------------
    
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
    
    cout << "--------" << " Testing charToInt " << "--------" <<  endl;
    int testInt = charToInt(testChar);
    cout << testInt << endl;
    cout << "----------" << endl;
    cout << endl;
    
    // Push back
    // --------------------------------
    
    cout << "--------" << " Testing pushBackChar " << "--------" <<  endl;
    char* testPushBack = new char[10];
    
    pushBackChar(testPushBack,'K');
    pushBackChar(testPushBack,'U');
    pushBackChar(testPushBack,'K');
    
    int size = strlen(testPushBack);
    
    cout << "Nr of chars: " << size << endl;
    
    for (int i = 0; i < size; i++) {
        cout << testPushBack[i] << endl;
    }
    
    cout << "--------" << " Testing to enter sensordata " << "--------" <<  endl;
    
    char* temp = new char[3];
    
    for (int i = 0; i < 3; i++) {
        temp[i] = 50-i;
    }
    
    testRobot->fwdValueIn(temp);
    testRobot->bwdValueIn(temp);
    testRobot->leftLongValueIn(temp);
    testRobot->leftFrontValueIn(temp);
    testRobot->leftBackValueIn(temp);
    testRobot->rightFrontValueIn(temp);
    testRobot->rightBackValueIn(temp);
    
    // Delete
    delete testRobot;
    delete mom;
    
	return 0;
}
