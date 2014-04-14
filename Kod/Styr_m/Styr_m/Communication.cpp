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
        robotPointer->changeGear('l');
		robotPointer->drive(25);
    }
    if (this->inData[1]=='r' && inData[2]=='1') {
        robotPointer->changeGear('r');
		robotPointer->drive(25);
    }
    if (this->inData[1]=='f'){
        int speed =(int)inData[3];
		robotPointer->changeGear('f');
        robotPointer->drive(25);
    }
    if (this->inData[1]=='b'){
        int speed =(int)inData[3];
		robotPointer->changeGear('b');
        robotPointer->driveBackward(25);
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