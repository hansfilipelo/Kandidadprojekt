#include "Communication.h"
#include "../../sensormodul/sensormodul/slave.h"
#include "MapSection.h"

Communication::Communication(Slave* pointer){
    slavePointer=pointer;
}

void Communication::handleData(){
    for (int i=0;i<25;i++){
        this->inData[i]=slavePointer->inDataArray[i];
    }
	//pillat med chars '0' och '1'
    dataLength = (int)inData[0];
    if (this->inData[1]=='r' && inData[2]=='0') {
        robotPointer->rotateLeft();
    }
    if (this->inData[1]=='r' && inData[2]=='1') {
        robotPointer->rotateRight();
    }
    if (this->inData[1]=='f'){
        int speed =(int)inData[3];
        robotPointer->driveForward(speed);
    }
    if (this->inData[1]=='b'){
        int speed =(int)inData[3];
        robotPointer->driveBackward(speed);
    }
    
    // Do stuff with sensor values
}

// -------------------

void Communication::setRobot(Robot* inRobot){
    robotPointer = inRobot;
}

// -------------------

void Communication::sendMap(){
    for (int i = 0; i < 32; i++) {
        char* sendData = robotPointer->getColAsChar(i);
        
        for (int i = 0; i < 20; i++) {
            slavePointer->outDataArray[i] = sendData[i];
        }
    }
}