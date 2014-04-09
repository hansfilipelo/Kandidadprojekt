#include "Communication.h"
#include "MapSection.h"

Communication::Communication(Slave* inSlave){
    slave = inSlave;
}


void Communication::handleData(){
    // Do stuff
}

// -------------------

void Communication::setRobot(Robot* inRobot){
    robot = inRobot;
}

// -------------------

void Communication::sendMap(){
    for (int i = 0; i < 32; i++) {
        char* sendData = robot->getColAsChar(i);
        
        for (int i = 0; i < 20; i++) {
            slave->outDataArray[i] = sendData[i];
        }
    }
}