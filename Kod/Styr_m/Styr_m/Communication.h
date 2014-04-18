#ifndef _Communication_h
#define _Communication_h

#include <stdio.h>
#include "slave.h"

#include "MapSection.h"
#include "Abstraction.h"

class MapSection;
class Robot;
class Slave;

class Communication {
public:
    Communication(Slave*);
    
    void handleData();
    void setRobot(Robot*);
    
    void sendMap();
    void sendRotateRequest();
    
//flyttade från protected
	unsigned char inData[27];
    unsigned char outData[27];
	volatile bool sendMapNow = false;
	int row = 0; 	   

protected:
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
	bool manual = true;
	
};

#endif
