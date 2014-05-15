#ifdef __APPLE__
#define TESTING 1
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
    mom->initMap();
    Slave* testSlave = new Slave();
    Communication* testComm = new Communication(testSlave);
    Robot* testRobot = new Robot(16,2,mom, testComm);
    testComm->setRobot(testRobot);
    
    // Map testing
    // -----------------------------
    cout << "--------" << " Printing map " << "--------" << endl;
    mom->printMap();
    cout << endl;
    
    if ( mom->getPos(16,1)->isClosed(16,1,0,-2) ) {
        cout << "Map is closed." << endl;
        
        mom->getPos(1,1)->cancer();
        mom->getPos(16,8)->cancer();
        mom->printMap();
    }
    else{
        cout << "Map is NOT closed." << endl;
    }
    
    // Delete
    delete testRobot;
    delete mom;
    
	return 0;
}
