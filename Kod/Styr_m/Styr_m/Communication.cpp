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
    // Protect from incorrect transmissions
    if (speed > 100) {
        speed = 5;
    }
    
	//checks manual/auto
	if (this->inData[1]=='a') {
		manual = false;
		robotPointer->setUserSpeed(speed);
		robotPointer->stopRotation();
		robotPointer->drive();
	}
	if (this->inData[1]=='q') {
		manual = true;
		robotPointer->setUserSpeed(0);
		robotPointer->stopRotation();
		robotPointer->drive();
	}
	//request to send map
	if (this->inData[1]=='m') {
		sendMapNow = true;
		this->row = inData[2];
		asm("");
	}
	
	if (this->inData[1]=='r' && inData[2]==0) {
		robotPointer->setRotateLeftActive();
	}
	else if (this->inData[1]=='r' && inData[2]==1) {
		robotPointer->setRotateRightActive();
	}
	else if (this->inData[1]=='f'){
		robotPointer->changeGear('f');
		robotPointer->setUserSpeed(speed);
		robotPointer->drive();
	}
	else if (this->inData[1]=='b'){
		robotPointer->changeGear('b');
		robotPointer->setUserSpeed(speed);
		robotPointer->drive();
	}
	else if (this->inData[1] == 'h'){
			robotPointer->setUserSpeed(0);
			robotPointer->stopRotation();
			robotPointer->drive();
				
	}
    
    
    if ( this->inData[1]=='S' ) {
        
        // Front sensor
        char temp[3];
        
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+3];
        }
        robotPointer->fwdLongValueIn(temp);
        
        // Back sensor
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+6];
        }
        robotPointer->bwdLongValueIn(temp);
        
        // Left back short sensor
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+9];
        }
        robotPointer->bwdShortValueIn(temp);
        
        // Right back sensor
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+12];
        }
        robotPointer->rightBackValueIn(temp);
        
        // Left front sensor
        for (int it = 0; it < 3 ; it++) {
            temp[it] = inData[it+15];
        }
        robotPointer->fwdShortValueIn(temp);
        
        // Right front sensor
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+18];
        }
        robotPointer->rightFrontValueIn(temp);
        
        
        // Right back sensor
        for (int it = 0; it < 3; it++) {
            temp[it] = inData[it+21];
        }
        robotPointer->leftLongValueIn(temp);
		robotPointer->newData = true;
        
    }
    
    // Gyro interrupt
    if (this->inData[1] == 'G') {
        robotPointer->stopRotation();
		robotPointer->robotRotated();
    }
	
	// RFID-detektion
    if (this->inData[1] == 'R') {
	    robotPointer->setRFID();
    }
    
    // Constants for PD-control, mapping and wall follower
    if(this->inData[1]=='P'){
		// PD-control
        double kp=assembleDouble(inData[3],inData[4],inData[5],inData[6]);
        double kd=assembleDouble(inData[7],inData[8],inData[9],inData[10]);
        int ref=(int)inData[11];
        int trimLeft = (int)inData[12];
        int trimRight = (int)inData[13];
		
		// Mapping
		int fwdRefLong = (int)inData[14];
		int bwdRefLong = (int)inData[15];
		int fwdRefShort = (int)inData[16];
		int bwdRefShort = (int)inData[16];
		
		// Wall follower
		int rightCornerFront = (int)inData[17];
		int rightCornerBack = (int)inData[18];
		int rightWallFront = (int)inData[19];
		int rightWallBack = (int)inData[20];
		int haltAfterSection = (int)inData[21];
		
        robotPointer->setControlParameters(kp,kd,ref,trimLeft,trimRight, fwdRefLong, bwdRefLong, fwdRefShort, bwdRefShort, rightCornerFront, rightCornerBack, rightWallFront, rightWallBack, haltAfterSection);
    }
    
}

// -------------------

bool Communication::getManual(){
    return manual;
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
    slavePointer->outDataArray[0] = 2;
    slavePointer->outDataArray[1] = 'g';
	slavePointer->outDataArray[2] = 1;
    
    slavePointer->SPI_Send();
}

double Communication::assembleDouble(char ten, char one, char tenth, char hundreth){
    double tenNumber=(double)ten*10;
    
	double oneNumber=(double)one;
	
	double tenthTemp = (double)tenth;
    volatile double tenthNumber=tenthTemp/10;
	
	double hundrethTemp = (double)hundreth;
    double hundrethNumber = hundrethTemp/100;
    
	volatile double output = tenNumber+oneNumber+tenthNumber+hundrethNumber;
	
    return output;
    
}

void Communication::reactivateRFID(){
	    slavePointer->outDataArray[0] = 1;
	    slavePointer->outDataArray[1] = 'r';
	    slavePointer->SPI_Send();
}
