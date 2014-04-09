#ifndef _Communication_h
#define _Communication_h

#include <stdio.h>
#include "../../../sensormodul/sensormodul/slave.h"

class Robot;

class Communication {
public:
    Communication(Slave*);
    
    void handleData();
    void setRobot(Robot*);
    
    void sendMap();
    
protected:
    Robot* robot = NULL;
    Slave* slave;
};

#endif
