#include "Communication.h"
#include "slave.h"
#include "MapSection.h"

Communication::Communication(Slave* pointer){
    slavePointer=pointer;
}

void Communication::handleData(){
    for (int i=0;i<25;i++){
        this->inData[i]=slavePointer->inDataArray[i];
    }
    dataLength = (int)inData[0];
	int speed = (int)inData[3];
	//checks manual/auto
	if (this->inData[1]=='a') {
		manual = false;
	}
	if (this->inData[1]=='q') {
		manual = true;
	}
	//request to send map
	if (this->inData[1]=='F') {
		sendMapNow = true;
	}
	
	if (manual){
		if (this->inData[1]=='r' && inData[2]==0) {
			robotPointer->changeGear('l');
			robotPointer->drive(speed);
		}
		else if (this->inData[1]=='r' && inData[2]==1) {
			robotPointer->changeGear('r');
			robotPointer->drive(speed);
		}
		else if (this->inData[1]=='f'){
			robotPointer->changeGear('f');
			robotPointer->drive(speed);
		}
		else if (this->inData[1]=='b'){
			robotPointer->changeGear('b');
			robotPointer->drive(speed);
		}
		else if (this->inData[1] == 'h'){
			robotPointer->drive(0);
		}
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
		
		slavePointer->outDataArray[0]=3;
		slavePointer->outDataArray[1]=1;
		slavePointer->outDataArray[2]=2;
		slavePointer->outDataArray[3]=3;
		
		//memcpy(slavePointer->outDataArray,robotPointer->getColAsChar(i),25);
       
		slavePointer->SPI_Send();
	}
}