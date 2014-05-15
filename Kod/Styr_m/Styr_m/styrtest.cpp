#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
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
    
    mom->initMap();
    mom->printMap();
    
    if (mom->getPos(16,1)->isClosed(16,1,0,-4)) {
        cout << "True!" << endl;
        mom->fillClosedArea();
        mom->printMap();
    }
    else {
        cout << "False... " << endl;
    }
    
    delete testRobot;
    delete mom;
    
	return 0;
}
