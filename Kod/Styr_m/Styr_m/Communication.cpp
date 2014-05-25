#include "Communication.h"
#include "slave.h"
#include "MapSection.h"

Communication::Communication(Slave* pointer){
    slavePointer=pointer;
}

// ------------------------------------
// Purpose: Handles all incoming data and sets flags in steer modules that later results in steer module handling of task

void Communication::handleData(){
    
    // Get data from bus slave
	memcpy(inData,slavePointer->inDataArray,27);
    // First bit is length according to protocol
    dataLength = (int)inData[0];
    
    // Many commands use speed
	int speed = (int)inData[3];
    // Protect from incorrect transmissions
	// does this work as intended
    if (speed > 100) {
        speed = 5;
    }
    
	//Sets automatic mode
	if (this->inData[1]=='a') {
		manual = false;
		robotPointer->stopRotation();
	}
    //Sets manual mode
	if (this->inData[1]=='q') {
		manual = true;
		robotPointer->setUserSpeed(0);
		robotPointer->stopRotation();
		robotPointer->drive();
	}
	//request to send map
	if (this->inData[1]=='m') {
        asm("");
        asm("");
		mapConfirmation = true;
        asm("");
        asm("");
	}
	
    // Rotate left
	if (this->inData[1]=='r' && inData[2]==0) {
		robotPointer->setRotateLeftActive();
	}
    // Rotate right
	else if (this->inData[1]=='r' && inData[2]==1) {
		robotPointer->setRotateRightActive();
	}
    // Drive forward
	else if (this->inData[1]=='f'){
		robotPointer->changeGear('f');
		robotPointer->setUserSpeed(speed);
		robotPointer->drive();
	}
    // Drive backwards
	else if (this->inData[1]=='b'){
		robotPointer->changeGear('b');
		robotPointer->setUserSpeed(speed);
		robotPointer->drive();
	}
    // Halt
	else if (this->inData[1] == 'h'){
			robotPointer->setUserSpeed(0);
			robotPointer->stopRotation();
			robotPointer->drive();
				
	}
    
    // Sensor data
    // Some references of sensors (fwdLonValueIn by example) that doesn't exist in final version.
    // These functions are stille needed so in order tp correct this there needs to be a small rewrite
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
        asm("");
        asm("");
		robotPointer->stopRotation();
		robotPointer->robotRotated();
		asm("");
		asm("");
	}
	
	// RFID-detection
    if (this->inData[1] == 'R') {
	    asm("");
	    asm("");
		isRFID=true;
		asm("");
		asm("");
	}
	
    // Moved one segment according to wheel counter
	if(this->inData[1] == 'W'){
		asm("");
		asm("");
		robotPointer->wheelHasTurned = true;
		robotPointer->rotationCount = 0;
		asm("");
		asm("");
	}
    
    // Request to get map from PC
    if( this->inData[1] == 'F' ){
        sendMapNow = true;
    }
    
    // Constants for PD-control
    if(this->inData[1]=='P'){
        double kp=assembleDouble(inData[3],inData[4],inData[5],inData[6]);
        double kd=assembleDouble(inData[7],inData[8],inData[9],inData[10]);
        int ref=(int)inData[11];
        int trimLeft = (int)inData[12];
        int trimRight = (int)inData[13];
		
        robotPointer->setControlParameters(kp,kd,ref,trimLeft,trimRight);
    }
    
}

// -------------------

bool Communication::getManual(){
    return manual;
}


// -------------------
// Needed for construction

void Communication::setRobot(Robot* inRobot){
    robotPointer = inRobot;
}

// -------------------

void Communication::sendMap(){
	volatile int k = 0; 
	for(unsigned int i = 0; i < 32; i++){
		sendRow(i);
		k = 0;
		while(!mapConfirmation && k < 5000){
			asm("");
            asm("");
            asm("");
            asm("");
			k++;
		}
#if TESTING == 0
		_delay_ms(10);
		mapConfirmation = false;
#endif
	}
}

void Communication::sendRow(unsigned int inRow){
	robotPointer->getColAsChar(inRow);
    memcpy(slavePointer->outDataArray,robotPointer->colArray,25);
    asm("");
    slavePointer->SPI_Send();
}

// --------------------------
// Asks for measure of turned angle to 90 degrees with gyro

void Communication::sendRotateRequest(){
    slavePointer->outDataArray[0] = 2;
    slavePointer->outDataArray[1] = 'g';
	slavePointer->outDataArray[2] = 1;
    
#if TESTING == 0
    slavePointer->SPI_Send();
	_delay_ms(5);
	slavePointer->SPI_Send();
	_delay_ms(5);
	slavePointer->SPI_Send();
#endif
}

// -------------------------------
// Makes a double out of one char per position

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


// ------------------------------------
// Reactivates RFID tag on sensor

void Communication::reactivateRFID(){
	    slavePointer->outDataArray[0] = 1;
	    slavePointer->outDataArray[1] = 'r';

#if TESTING == 0
	    slavePointer->SPI_Send();
		_delay_ms(5);
		slavePointer->SPI_Send();
		_delay_ms(5);
		slavePointer->SPI_Send();
#endif
}

// ------------------------------------
// Reactivates wheel counter

void Communication::reactivateWheelSensor(){
	slavePointer->outDataArray[0] = 1;
	slavePointer->outDataArray[1] = 'w';
	slavePointer->SPI_Send();

#if TESTING == 0
	_delay_ms(5);
	if(!robotPointer->wheelHasTurned){
		slavePointer->SPI_Send();
	}
	
	_delay_ms(5);
	if(!robotPointer->wheelHasTurned){
		slavePointer->SPI_Send();
	}
#endif
}

// -----------------------------------
// Sends path A-star algorith suggests to PC for debugging. Not implemented, see tech doc

void Communication::sendAStar(char* inArray)
{
	int sizeOfArray = inArray[0]- 48;
	int timesToSend = 1 + sizeOfArray/20;
	
	slavePointer->outDataArray[0] = 22;
	slavePointer->outDataArray[1] = 't';	
	
	for(int k=0; k<timesToSend; k++){
		slavePointer->outDataArray[2] = k;
		slavePointer->outDataArray[3] = sizeOfArray;
		
		for (int i=1; i<20;i++)
		{
			slavePointer->outDataArray[i+3] = inArray[k*20+i];
		}
		slavePointer->SPI_Send();
#if TESTING == 0
		_delay_ms(40);
#endif
	}
}
