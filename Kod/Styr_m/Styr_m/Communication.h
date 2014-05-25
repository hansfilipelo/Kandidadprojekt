/*
 File: Communication.h
 Purpose: Class working as abstraction layer between the logical (software) robot and the bus slave. 
 */

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
	void sendAStar(char*);
    
    // Used for buffer
	unsigned char inData[27];
    unsigned char outData[27];
	
    volatile bool sendMapNow = false;
	// COunter for sending map row by row
    int row = 0;
    // Makes a double out of one char per position
    double assembleDouble(char, char, char, char);
	bool getManual();
    volatile bool isRFID=false;
    
protected:
    
    void sendRow(unsigned int);
    volatile bool mapConfirmation = false;
	bool manual = true;
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
	
	
};

#endif
