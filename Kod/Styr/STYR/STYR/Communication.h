#ifndef _Communication_h
#define _Communication_h


#include "MapSection.h"
#include "Abstraction.h"

class Slave;
class Robot;


class Communication {
public:
    Communication(Slave* pointer1,Robot* pointer2 );
    void handleData();
    
    
protected:
    int dataLength;
    Slave* slavePointer;
    Robot* robotPointer;
    unsigned char inData[25];
    unsigned char outData[25];
    
};

#endif
