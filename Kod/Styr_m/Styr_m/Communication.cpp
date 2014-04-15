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
		asm("");
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
    
    // Front sensor
    if (this->inData[1] == 'S' && this->inData[2] == 0) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->fwdValueIn(temp);
    }
    
    // Back sensor
    if (this->inData[1] == 'S' && this->inData[2] == 1) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->bwdValueIn(temp);
    }
    
    // Left long sensor
    if (this->inData[1] == 'S' && this->inData[2] == 6) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->leftLongValueIn(temp);
    }
    
    // Left front sensor
    if (this->inData[1] == 'S' && this->inData[2] == 2) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->leftFrontValueIn(temp);
    }
    
    // Left back sensor
    if (this->inData[1] == 'S' && this->inData[2] == 3) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->leftBackValueIn(temp);
    }
    
    // Right front sensor
    if (this->inData[1] == 'S' && this->inData[2] == 4) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->rightFrontValueIn(temp);
    }
    
    // Right back sensor
    if (this->inData[1] == 'S' && this->inData[2] == 5) {
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->rightBackValueIn(temp);
    }
    
    // Gyro interrupt
    if (this->inData[1] == 'G') {
        robotPointer->stopRotation();
    }
    
}

// -------------------

void Communication::setRobot(Robot* inRobot){
    robotPointer = inRobot;
}

// -------------------

void Communication::sendMap(){
    //for (int i = 0; i < 32; i++) {
		
		slavePointer->outDataArray[0]=3;
		slavePointer->outDataArray[1]='M';
		slavePointer->outDataArray[2]='U';
		slavePointer->outDataArray[3]='U';
		
		//memcpy(slavePointer->outDataArray,robotPointer->getColAsChar(i),25);
       
		slavePointer->SPI_Send();
//	}
}

// --------------------------
// Asks for measure of angles

void Communication::sendRotateRequest(){
    slavePointer->outDataArray[0] = 1;
    slavePointer->outDataArray[1] = 'g';
    
    slavePointer->SPI_Send();
}



