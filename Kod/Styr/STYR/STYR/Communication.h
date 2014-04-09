#ifndef _Communication_h
#define _Communication_h

#include <stdio.h>

class Robot;

class Communication {
public:
    
    
    void handleData();
    void setRobot(Robot*);
    
protected:
    Robot* robot = NULL;
};

#endif
