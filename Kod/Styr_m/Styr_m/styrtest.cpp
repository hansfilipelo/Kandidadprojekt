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
    
    mom->initMap();
    mom->printMap();
    
    cout << mom->getPos(1,1)->isClosed(1,1) << endl;
    
    delete testRobot;
    delete mom;
    
	return 0;
}
