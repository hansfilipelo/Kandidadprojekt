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
//flyttade från protected
	unsigned char inData[25];
    unsigned char outData[25];
	volatile bool sendMapNow = false;	   

protected:
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
	bool manual = true;
	
};

#endif
