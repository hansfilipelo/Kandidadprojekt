#include "Communication.h"
#include "slave.h"
#include "MapSection.h"

Communication::Communication(Slave* pointer){
    slavePointer=pointer;
}

void Communication::handleData(){
    
	memcpy(inData,slavePointer->inDataArray,27);
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
	if (this->inData[1]=='m') {
		sendMapNow = true;
		this->row = inData[2];
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
    
    
    if ( this->inData[1]=='S' ) {
        
        // Front sensor
        char* temp = new char[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->fwdValueIn(temp);
        
        // Back sensor
        for (int it = 3; it < 6; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->bwdValueIn(temp);
        
        // Left long sensor
        for (int it = 6; it < 9; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->leftLongValueIn(temp);
        
        // Left front sensor
        for (int it = 9; it < 12; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->leftFrontValueIn(temp);
        
        // Left back sensor
        for (int it = 12; it < 15 ; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->leftBackValueIn(temp);
        
        // Right front sensor
        for (int it = 15; it < 18; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->rightFrontValueIn(temp);
        
        
        // Right back sensor
        for (int it = 18; it < 21; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->rightBackValueIn(temp);
        
    }
    
    // Gyro interrupt
    if (this->inData[1] == 'G') {
        robotPointer->stopRotation();
    }
    
    // Constants for PD-control
    if(this->inData[1]=='P'){
        int kp=assembleDouble(inData[3],inData[4],inData[5],inData[6]);
        int kd=assembleDouble(inData[7],inData[8],inData[9],inData[10]);
        int ref=(int)inData[11];
                          
        robotPointer->setControlParameters(kp,kd,ref);
    }
    
}

// -------------------

void Communication::setRobot(Robot* inRobot){
    robotPointer = inRobot;
}

// -------------------

void Communication::sendMap(){
    memcpy(slavePointer->outDataArray,robotPointer->getColAsChar(row),25);
    asm("");
    slavePointer->SPI_Send();
}

// --------------------------
// Asks for measure of angles

void Communication::sendRotateRequest(){
    slavePointer->outDataArray[0] = 1;
    slavePointer->outDataArray[1] = 'g';
    
    slavePointer->SPI_Send();
}

double assembleDouble(char ten, char one, char tenth, char hundreth){
    double tenNumber=(double)ten*10;
    double oneNumber=(double)one;
    double tenthNumber=(double)tenth/10;
    double hundrethNumber=(double)hundreth/100;
    
    return tenNumber+oneNumber+tenthNumber+hundrethNumber;
    
}


