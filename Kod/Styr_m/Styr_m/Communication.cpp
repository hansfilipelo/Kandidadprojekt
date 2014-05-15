#include "Communication.h"
#include "slave.h"
#include "MapSection.h"

Communication::Communication(Slave* pointer){
    slavePointer=pointer;
	wheelHasTurned = false;
}

/*
 *  Data handling for steermodule.
 */

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
	}
	if (this->inData[1]=='q') {
		manual = true;
		robotPointer->setUserSpeed(0);
		robotPointer->stopRotation();
		robotPointer->drive();
	}
	//request to send map
	if (this->inData[1]=='m') {
		/*this->row = inData[2];
		sendMap();*/
		mapConfirmation = true;
	}
	
	if (this->inData[1]=='r' && inData[2]==0) {
		robotPointer->setRotateLeftActive();
	}
	else if (this->inData[1]=='r' && inData[2]==1) {
		robotPointer->setRotateRightActive();
	}
	else if (this->inData[1]=='f'){
		wheelHasTurned=false; 
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
    
    if( this->inData[1] == 'F' ){
        sendMapNow = true;
    }
    
    
    if ( this->inData[1]=='S' ) {
        
        sensorArrayToVariables();
		robotPointer->newData = true;
		asm("");
		asm("");
		
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
	
	//Åkt 40 cm
    if (this->inData[1] == 'D') {
		asm("");
		wheelHasTurned=true; 
		asm("");
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
		int bwdRefShort = (int)inData[17];
		
		// Wall follower
		int rightCornerFront = (int)inData[18];
		int rightCornerBack = (int)inData[19];
		int rightWallFront = (int)inData[20];
		int rightWallBack = (int)inData[21];
		int haltAfterSection = (int)inData[22];
		int Kp2 =(int)inData[23];
		
		asm("");
        robotPointer->setControlParameters(kp,kd,ref,trimLeft,trimRight, fwdRefLong, bwdRefLong, fwdRefShort, bwdRefShort, rightCornerFront, rightCornerBack, rightWallFront, rightWallBack, haltAfterSection,Kp2);
    }
    
}

// ----------Manual accessfunction ----------------
bool Communication::getManual(){
    return manual;
}


// -----------------------------------------------


void Communication::setRobot(Robot* inRobot){
    robotPointer = inRobot;
}


// -----------------------------------------------
/*
 *  sendMap(), send all 31 rows, after each transfer wait for a mapConfirmation.
 *  When we've received the confirmation we set it to false and reiterate.
 */

void Communication::sendMap(){
	for(unsigned int i = 0; i < 32; i++){
		sendRow(i);
		while(!mapConfirmation){
			asm("");
		}
		mapConfirmation = false;
	}
}

// -----------------------------------------------

/*
 *  copy the row into the outDataArray and send the transmission interrupt to master.
 */

void Communication::sendRow(unsigned int inRow){
    memcpy(slavePointer->outDataArray,robotPointer->getColAsChar(inRow),25);
    asm("");
    slavePointer->SPI_Send();
}

/* Not used in current implementation.
void Communication::initSendMap(){
	slavePointer->outDataArray[0] = 1;
	slavePointer->outDataArray[1] = 'F';
	slavePointer->SPI_Send();
}
 
*/

// --------------------------
// Asks for measure of angles

void Communication::sendRotateRequest(){
    slavePointer->outDataArray[0] = 2;
    slavePointer->outDataArray[1] = 'g';
	slavePointer->outDataArray[2] = 1;
   
    slavePointer->SPI_Send();
}

/*
 *  The doubles are sent in separate chars (tens, ones, tenths and hundreths) 
 *  this function reassembles the double.
 */

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

/*
 *  When a rfid is detected to avoid detecting multiple in same square the rfid
 *  detection is deactivated. This command reactivates RFID-detection
 */

void Communication::reactivateRFID(){
	    slavePointer->outDataArray[0] = 1;
	    slavePointer->outDataArray[1] = 'r';
	    slavePointer->SPI_Send();
}

/*
 *  Function for reassembling of sensorvalues from array to internal variables.
 */


void Communication::sensorArrayToVariables(){
    
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
}

//activera hjulräknaren
void Communication::activateWheelSensor(){
	slavePointer->outDataArray[0] = 1;
	slavePointer->outDataArray[1] = 'd';
	//slavePointer->SPI_Send();
}

void Communication::time0()
{
	slavePointer->outDataArray[0] = 2;
	slavePointer->outDataArray[1] = 'T';
	slavePointer->outDataArray[2] = 0;
	slavePointer->SPI_Send();
}

void Communication::time1()
{
	slavePointer->outDataArray[0] = 2;
	slavePointer->outDataArray[1] = 'T';
	slavePointer->outDataArray[2] = 1;
	slavePointer->SPI_Send();
}
