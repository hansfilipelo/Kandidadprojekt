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
    dataLength = (int)inData[0];
	int speed =10*(int)inData[3] + 10;
    if (this->inData[1]=='r' && inData[2]==0) {
        robotPointer->changeGear('l');
		robotPointer->drive(25);
    }
    if (this->inData[1]=='r' && inData[2]==1) {
        robotPointer->changeGear('r');
		robotPointer->drive(25);
    }
    if (this->inData[1]=='f'){
		robotPointer->changeGear('f');
        robotPointer->drive(speed);
    }
    if (this->inData[1]=='b'){
		robotPointer->changeGear('b');
        robotPointer->drive(speed);
    }
	if (this->inData[1] == 'h'){
		robotPointer->drive(0);
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