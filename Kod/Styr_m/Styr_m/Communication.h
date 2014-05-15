#ifndef _Communication_h
#define _Communication_h

#define F_CPU 14745600

#include <stdio.h>
#include "slave.h"

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
	void reactivateRFID();
	void reactivateWheelSensor();
    
//flyttade från protected
	unsigned char inData[27];
    unsigned char outData[27];
	volatile bool sendMapNow = false;
	int row = 0;
    double assembleDouble(char, char, char, char);
	bool getManual();
    
    
protected:
    
    
	bool manual = true;
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
	
};

#endif
