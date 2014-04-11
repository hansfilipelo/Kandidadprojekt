#ifndef _Communication_h
#define _Communication_h

#include <stdio.h>
#include "../../sensormodul/sensormodul/slave.h"

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
    
protected:
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
    unsigned char inData[25];
    unsigned char outData[25];
    
};

#endif
