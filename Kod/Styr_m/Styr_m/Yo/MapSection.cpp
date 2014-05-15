#include "MapSection.h"
#include "Map.h"

using namespace std;

// Construct -------------------

MapSection::MapSection(int x,int y, Map* inMom){
    xCoord=x;
    yCoord=y;
    mom = inMom;
    type='u';
}

MapSection::MapSection(int x,int y, Map* inMom, char createType){
    xCoord=x;
    yCoord=y;
    mom = inMom;
    type=createType;
}

// Destruct

MapSection::~MapSection(){};

// ---------------------------------------------------------
// Set and get MapSection coordinates

void MapSection::setX(int coord){
	xCoord=coord;
}

void MapSection::setY(int coord){
	yCoord=coord;
}

void MapSection::setType(char newType){
	type=newType;
}

int MapSection::getX(){
	return this->xCoord;
}

int MapSection::getY(){
	return this->yCoord;
}

char MapSection::getType(){
	return type;
}

//---------------------------------------------------------

MapSection* MapSection::getTop(){
	return mom->getPos(xCoord, yCoord + 1);
}

MapSection* MapSection::getBottom(){
	return mom->getPos(xCoord, yCoord - 1);
}

MapSection* MapSection::getLeft(){
	return mom->getPos(xCoord - 1, yCoord);
}

MapSection* MapSection::getRight(){
	return mom->getPos(xCoord + 1, yCoord);
}

//-----------------------------------------------------------



//----------------------------------------------------------
//Checks if the current MapSection is of type unexplored

bool MapSection::isUnexplored(){
	if (type == 'u') {
		return true;
	}
	else{
		return false;
	}
}

// ----------------------------------------------------------
// SLAM algorithms
//Looks at adjacent MapSections and finds the closest unexplored section to optimize pathing.
//Counters are compared and used to find the shortest path.

int MapSection::findUnexplored(){
	int topCounter;
	int botCounter;
	int leftCounter;
	int rightCounter;
	
	if (getTop()->isUnexplored()){
		topCounter=1;
	}
	else{
		topCounter= getTop()->findUnexplored()+1;
	}
	
	if (getBottom()->isUnexplored()){
		botCounter=1;
	}
	else{
		botCounter= getBottom()->findUnexplored()+1;
	}
	
	if (getLeft()->isUnexplored()){
		leftCounter=1;
	}
	else{
		leftCounter= getLeft()->findUnexplored()+1;
	}
	
	if (getRight()->isUnexplored()){
		rightCounter=1;
	}
	else{
		rightCounter= getRight()->findUnexplored()+1;
	}
	
	if(topCounter < botCounter || topCounter < leftCounter || topCounter < rightCounter){
		next = 't';
		return topCounter;
	}
	else if (botCounter < leftCounter || botCounter < rightCounter){
		next = 'b';
		return botCounter;
	}
	else if (leftCounter < rightCounter){
		next = 'l';
		return leftCounter;
	}
	else {
		next = 'r';
		return rightCounter;
	}
}

// ------------------------
// Checks if closed area is closed

bool MapSection::isClosed(int origX, int origY, int counter){
    
    if ( ((abs(xCoord-origX) < 2) && (abs(yCoord-origY) < 2)) && counter > 10 ) {
        return true;
    }
    
    int nextX;
    int nextY;
    
    this->hasBeenClosed = true;

    // Using clock as direction pointer in comments
    // Check 9
    if ((xCoord - 1 > 0) && (yCoord > 0) && (xCoord - 1 < 18) && (yCoord < 32) && mom->getPos(xCoord - 1, yCoord)->getType() == 'c' && !mom->getPos(xCoord - 1, yCoord)->hasBeenClosed ) {
        
        nextX = xCoord - 1;
        nextY = yCoord;
	}
    // Check 10,5
    else if ( (xCoord - 1 > 0) && (yCoord - 1 > 0) && (xCoord - 1 < 18) && (yCoord - 1 < 32) && mom->getPos(xCoord - 1, yCoord - 1)->getType() == 'c' && !mom->getPos(xCoord - 1, yCoord - 1)->hasBeenClosed ) {
        
        nextX = xCoord - 1;
        nextY = yCoord - 1;
	}
    // Check 12
    else if ( (xCoord > 0) && (yCoord - 1 > 0) && (xCoord < 18) && (yCoord - 1 < 32) && mom->getPos(xCoord, yCoord - 1)->getType() == 'c' && !mom->getPos(xCoord, yCoord - 1)->hasBeenClosed ) {
        
        nextX = xCoord;
        nextY = yCoord - 1;
	}
    // Check 1,5
    else if ((xCoord + 1 > 0) && (yCoord - 1 > 0) && (xCoord + 1 < 18) && (yCoord - 1 < 32) && mom->getPos(xCoord + 1, yCoord - 1)->getType() == 'c' && !mom->getPos(xCoord + 1, yCoord - 1)->hasBeenClosed ) {
        
        nextX = xCoord + 1;
        nextY = yCoord - 1;
	}
    // Check 3
    else if ((xCoord + 1 > 0) && (yCoord > 0) && (xCoord + 1 < 18) && (yCoord < 32) && mom->getPos(xCoord + 1, yCoord)->getType() == 'c' && !mom->getPos(xCoord + 1, yCoord)->hasBeenClosed ) {
        
        nextX = xCoord + 1;
        nextY = yCoord;
	}
    // Check 4,5
    else if ((xCoord + 1 > 0) && (yCoord + 1 > 0) && (xCoord + 1 < 18) && (yCoord + 1 < 32) && mom->getPos(xCoord + 1, yCoord + 1)->getType() == 'c' && !mom->getPos(xCoord + 1, yCoord + 1)->hasBeenClosed ) {
        
        nextX = xCoord + 1;
        nextY = yCoord + 1;
	}
    // Check 6
    else if ((xCoord > 0) && (yCoord + 1 > 0) && (xCoord < 18) && (yCoord + 1 < 32) && mom->getPos(xCoord, yCoord + 1)->getType() == 'c' && !mom->getPos(xCoord, yCoord + 1)->hasBeenClosed ) {
        
        nextX = xCoord;
        nextY = yCoord + 1;
	}
    // Check 7,5
    else if ((xCoord - 1 > 0) && (yCoord + 1 > 0) && (xCoord - 1 < 18) && (yCoord + 1 < 32) && mom->getPos(xCoord - 1, yCoord + 1)->getType() == 'c' && !mom->getPos(xCoord - 1, yCoord + 1)->hasBeenClosed ) {
        
        nextX = xCoord - 1;
        nextY = yCoord + 1;
    }
    else {
		return false;
    }
    
    return mom->getPos(nextX, nextY)->isClosed(origX, origY, counter+1);
    
}


/*	-------------------------------------------------------------

						ROBOT
					
	-----------------------------------------------------------*/

// Construct ---------------------------
// Since this is a subclass - the MapSection constructor runs first.
// All we need to do is change type and set ourselves on map
//Also initiates default values on certain parameters used in robot control.

Robot::Robot(int xPos, int yPos, Map* inMom, Communication* inComm) : MapSection(xPos, yPos, inMom){
	type = 'r';
	direction = 'f';
	
	Kd = 23;
	Kp = 7;
	Ref = 12;
	
	trimRight = 30;
	trimLeft = 0;
	
	fwdRefLong = 26;
	bwdRefLong = 0;
	
	fwdRefShort = 28;
	bwdRefShort = 10; 
	
	rightCornerFront = 40;
	rightCornerBack = 25;
	
	rightWallFront = 30;
	rightWallBack = 30;
	
	haltAfterSection = false;
	
	rotateRightActive = false;
	rotateLeftActive = false; 
    
    commObj = inComm;
    previousSection = mom->getPos(xPos,yPos);
    mom->setSection(xPos,yPos,this);
}

Robot::~Robot(){
    mom->setSection(xCoord,yCoord,new MapSection(xCoord,yCoord,mom));
}

// -------------------------------------
// Changes the direction robot is moving in by adjusting PWM

void Robot::changeGear(char inGear){
		
        currentGear = inGear;
        if (inGear == 'b'){
            
            gear = inGear;
            
            #if DEBUG == 0
			PORTD &= ~(1<<PORTD4); //0x10;
			PORTD &= ~(1<<PORTD5); //0x20;
            #endif
		}
		else if (inGear == 'r'){
            gear = inGear;
            
            #if DEBUG == 0
			PORTD &= ~(1<<PORTD5);
			PORTD |= (1<<PORTD4);
            #endif
		}
		else if (inGear == 'l'){
            gear = inGear;
            
            #if DEBUG == 0
			PORTD &= ~(1<<PORTD4);
			PORTD |= (1<<PORTD5);
            #endif
		}
		else if (inGear == 'f'){
            gear = inGear;
            
            #if DEBUG == 0
			PORTD |= (1<<PORTD4);
			PORTD |= (1<<PORTD5);
            #endif
		}
}

// ------------------------------------
// Changes previous MapSection to typ "fire"
void Robot::setRFID(){
	previousSection->setType('f');
}


// Changes the PWM outputs by using the user defined speed and the user set trim values.
//Trim values are used to adjust how straight the robot drives.
void Robot::drive(){
	if (speed < 101)
    {
        int outputLeft;
        int outputRight;
        
        if (currentGear == 'f' && speed != 0){
            outputLeft = floor(speed * (255 + trimLeft) / 100);
            outputRight = floor(speed * (255 + trimRight) / 100);
        }
        else{
            outputLeft = floor(speed * 255 / 100);
            outputRight = floor(speed * 255 /100);
		}
		#if DEBUG == 0
		OCR2A = outputLeft;
		OCR2B = outputRight;
		#endif
	}
	else {
#if DEBUG == 0
		OCR2A = 0;
		OCR2B = 0;
#endif
	}
}

//Same as drive but backwards
void Robot::driveBackward(int speed){
    changeGear('b');
	int output = floor(speed * 255 / 100);
	
#if DEBUG == 0
    OCR2A = output;
    OCR2B = output;
#endif
}

// ------------------------------------
// Rotates robot

void Robot::rotateLeft(){
	// Rotate mode
	rotateLeftActive = false;
	rotateActive = true;
	// Seft diffs to 0
	
	fwdDiff = 0;
	bwdDiff = 0;
	
	// Send map before rotating since it's the least critical point during mapping run
	setSpeed(0);
	drive();
	//commObj->sendMap();
	
    // Send request to sensor module to measure angle
    commObj->sendRotateRequest();
	
    // Turns
    changeGear('l');
    setSpeed(35);
	
	while (rotateActive)
	{
		drive();
	}
	
	// Stop rotation and set gear to forward
	setSpeed(0);
	changeGear('f');
	drive();
    
    // Update direction
    if (direction == 'f') {
        changeDirection('l');
    }
    else if (direction == 'l') {
        changeDirection('b');
    }
    else if (direction == 'b') {
        changeDirection('r');
    }
    else if (direction == 'r') {
        changeDirection('f');
    }
	
	waitForNewData();
	this->robotRotated();
		
}

// Stops rotation
void Robot::stopRotation(){
    rotateActive = false;
}

// ------------------------------------

void Robot::rotateRight(){
	// Rotate mode
	rotateRightActive = false;
	rotateActive = true;
	// Seft diffs to 0
	fwdDiff = 0;
	bwdDiff = 0;
	
	// Send map before rotating since it's the least critical point during mapping run
	setSpeed(0);
	drive();
	//commObj->sendMap();
	
	//---------
	// First send stuff to sensor module
	// When we have rotated 90 degrees sensor module will send a signal which will deactivate rotate
	//---------
	commObj->sendRotateRequest();
	
    // Turns
    changeGear('r');
    setSpeed(35);
	while (rotateActive)
	{
 		drive();
	}
	
	// Stop rotation and set gear to forward
	setSpeed(0);
	changeGear('f');
	drive();
    
    // Update direction
    if (direction == 'f') {
        changeDirection('r');
    }
    else if (direction == 'r') {
        changeDirection('b');
    }
    else if (direction == 'b') {
        changeDirection('l');
    }
    else if (direction == 'l') {
        changeDirection('f');
    }
	waitForNewData();
	this->robotRotated();
	
}

//-----------------------------------------
//
void Robot::turn(int pd){
    int output = floor(movementSpeed * 255 / 100);
	
	int pdOut = pd * movementSpeed * 0.01;
	
	#if DEBUG == 0
	OCR2A = output+pdOut;
	OCR2B = output-pdOut;
	#endif
}


// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdLongValueIn(char fwd[3]){
    fwdLongSensor = 100 * fwd[0];
    fwdLongSensor += 10 * fwd[1];
    fwdLongSensor += fwd[2];

#if DEBUG == 1
    cout << "fwdValueIn" << endl;
    cout << fwdLongSensor << endl;
#endif
}

void Robot::bwdLongValueIn(char* bwd){
    bwdLongSensor = 100 * bwd[0];
    bwdLongSensor += 10 * bwd[1];
    bwdLongSensor += bwd[2];
    
#if DEBUG == 1
    cout << "bwdValueIn" << endl;
    cout << bwdLongSensor << endl;
#endif
}

void Robot::bwdShortValueIn(char bwdShort[3]){
    bwdShortSensor = 100 * bwdShort[0];
    bwdShortSensor += 10 * bwdShort[1];
    bwdShortSensor += bwdShort[2];
    
#if DEBUG == 1
    cout << "bwdShortValueIn" << endl;
    cout << bwdShortSensor << endl;
#endif
}

void Robot::fwdShortValueIn(char fwdShort[3]){
    fwdShortSensor = 100 * fwdShort[0];
    fwdShortSensor += 10 * fwdShort[1];
    fwdShortSensor += fwdShort[2];
    
#if DEBUG == 1
    cout << "fwdShortValueIn" << endl;
    cout << fwdShortSensor << endl;
#endif
}

void Robot::leftLongValueIn(char left[3]){
    leftMidSensor = 100 * left[0];
    leftMidSensor += 10 * left[1];
    leftMidSensor += left[2];
    
#if DEBUG == 1
    cout << "leftLongValueIn" << endl;
    cout << leftMidSensor << endl;
#endif
}

void Robot::rightBackValueIn(char right[3]){
	rightBackSensor = 100 * right[0];
    rightBackSensor += 10 * right[1];
    rightBackSensor += right[2];
    
#if DEBUG == 1
    cout << "rightBackValueIn" << endl;
    cout << rightBackSensor << endl;
#endif
}

void Robot::rightFrontValueIn(char right[3]){
    rightFrontSensor = 100 * right[0];
    rightFrontSensor += 10 * right[1];
    rightFrontSensor += right[2];
    
#if DEBUG == 1
    cout << "rightFrontValueIn" << endl;
    cout << rightFrontSensor << endl;
#endif
}

// --------------------------------------------
// Sets walls in Map

void Robot::setFwdClosed(){
	
    int output = 0;
    if (getFwdDistance() > 300) {
        output = 280/40;
    }
    else{
        output = getFwdDistance()/40;
    }
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
        
        // Set every section between robot and wall as empty
		for (int i = 0; i < output; i++) {
            if(yCoord+i+1>16){
                break;
            }
			if(mom->getPos(xCoord,yCoord + i + 1)->getType() != 'f'){
				 mom->convertSection(xCoord,yCoord + i + 1, 'e');
			}
		}
        if(output == 0){
	        mom->convertSection(xCoord,yCoord + 1, 'c');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-i-1<0){
                break;
            }
			if(mom->getPos(xCoord,yCoord - i - 1)->getType() != 'f'){
				mom->convertSection(xCoord,yCoord - i - 1, 'e');
			}
        }
		if(output == 0){
			mom->convertSection(xCoord,yCoord - 1, 'c');
		}
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){									//left right kan vara omv�nt, m�ste testas
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord+i+1>31){
                break;
            }
			if(mom->getPos(xCoord + i + 1,yCoord)->getType() != 'f'){
				mom->convertSection(xCoord + i + 1,yCoord, 'e');
			}
        }
		if(output == 0){
			mom->convertSection(xCoord + 1,yCoord, 'c');
		}
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord-i-1<0){
                break;
            }
			if(mom->getPos(xCoord - i - 1,yCoord)->getType() != 'f'){
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
			}
        }
		if(output == 0){
			mom->convertSection(xCoord - 1,yCoord, 'c');
		}
	}	
}

// ------------------------------------------------

void Robot::setBwdClosed(){
	
	int output = 0;
	
	if (getBwdDistance() > 300) {
		output = 280/40;
	}
	else{
		output = getBwdDistance()/40; //ser vissa problem med detta.
	}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-1-i < 0){
                break;
            }
			if(mom->getPos(xCoord,yCoord - i - 1)->getType() != 'f'){
				mom->convertSection(xCoord,yCoord - i - 1, 'e');
			}
        }
		if(output == 0){
			mom->convertSection(xCoord,yCoord - 1, 'c');
		}
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord +1 +i >16){
                break;
            }
			if(mom->getPos(xCoord,yCoord + i + 1)->getType() != 'f'){
				mom->convertSection(xCoord,yCoord + i + 1, 'e');
			}
		}
		if(output == 0){
			mom->convertSection(xCoord,yCoord + 1, 'c');
		}
	}
	// Direction 0->x->32, "right"
	else if (direction == 'l'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord-1-i < 0){
                break;
            }
			if(mom->getPos(xCoord - i - 1,yCoord)->getType() != 'f'){
				mom->convertSection(xCoord - i - 1,yCoord, 'e');
			}
        }
		if(output == 0){
			mom->convertSection(xCoord - 1,yCoord, 'c');
		}
	}
	// Direction 32->x->0, "left"
	else if (direction == 'r'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord+1+i > 31){
                break;
            }
			if(mom->getPos(xCoord + i + 1,yCoord)->getType() != 'f'){
				mom->convertSection(xCoord + i + 1,yCoord, 'e');
			}
        }
		if(output == 0){
			mom->convertSection(xCoord + 1,yCoord, 'c');
		}
	}
}

// -------------- To the left --------------------------

void Robot::setLeftClosed(){
	
		int output = 0;
	    
	    if(leftMidSensor < 40){
		    output = 10/40;
	    }
	    else if(leftMidSensor > 150) { // this value might need to be calibrated
		    output = 160/40;				//if distance is great only print max 2 empty.
	    }

	    else{
		    output = leftMidSensor/40;
	    }

	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
        
        // Set every section between robot and wall as empty or closed
        for (int i = 0; i < output; i++) {
            if(xCoord+1+i > 31){
                break;
            }
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
        if(output == 0){
	        mom->convertSection(xCoord + 1,yCoord, 'c');
        }
	}

		
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
        
        // Set every section between robot and wall as empty or closed
        for (int i = 0; i < output; i++) {
            if(xCoord-1-i < 0){
                break;
            }
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
		if(output == 0){
			mom->convertSection(xCoord - 1,yCoord, 'c');
		}
	}
	// Direction 0->x->32, "right"
	else if (direction == 'l'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-1-i < 0){
                break;
            }
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
		if(output == 0){
			mom->convertSection(xCoord,yCoord - 1, 'c');
		}
	}
	// Direction 32->x->0, "left"
	else if (direction == 'r'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord+1+i > 16){
                break;
            }
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
		if(output == 0){
			mom->convertSection(xCoord,yCoord + 1, 'c');
		}
	}
}

// -------------- To the right --------------------------

void Robot::setRightClosed(){
	
	int output = 0;
    
    /*if( getRightDifference() < -5 || getRightDifference() > 5){
        return; //the too great uncertainty if.
    }*/
	
	if ( rightFrontSensor > 60 ) { // this value might need to be calibrated
		output = 80/40;//if distance is great only print max 2 empty.
	}
	else{
		output = rightFrontSensor/40;
	}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord-1-i < 0){
                break;
            }
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
        if(output == 0){
            mom->convertSection(xCoord - 1,yCoord, 'c');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord+1+i > 31){
                break;
            }
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
        if(output == 0){
            mom->convertSection(xCoord + 1,yCoord, 'c');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'l'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord+1+i > 16){
                break;
            }
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
        if(output == 0){
            mom->convertSection(xCoord,yCoord + 1, 'c');
        }
        
	}
	// Direction 32->x->0, "left"
	else if (direction == 'r'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-1-i < 0){
                break;
            }
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
        if(output == 0){
            mom->convertSection(xCoord,yCoord - 1, 'c');
        }
	}
}

//------------------------------------------------------

int Robot::meanValueArray(char* inputArray, int iterations) {
    // Create reasonable valid data from latest reads.
    int total = 0;
	int iter = iterations;
	
	for (int i = 0; i < iter; i++){
		total = total + getValueFromArray(inputArray,i);
	}
	
	return total / iter;
}


// -----------------------------------------

//Sets reference values and moves robot in map abstraction if robot has moved one square
void Robot::updateRobotPosition(){
	
	waitForNewData();
	
	if(validSensor == 'N'){
		validSensor = determineValidSensor();
	}
	
	//b�da m�ste ske flera g�nger f�r att byta segment, sensorerna kan ge extremv�rden som leder till f�r tidigt bytt ruta
	if (validSensor == 'w'){
		return;
	}
	
	if (validSensor == 'b'){
		int ref = bwdReference/40;
		sensorDifference = getBwdDistance() - ref*40;
	}
	else if(validSensor == 'f'){
		int ref = fwdReference/40;
		sensorDifference = getFwdDistance() - ref*40;
	}
	
	int fwdref = 0;
	int bwdref = 0;
	
	if(usingLong){
		fwdref = fwdRefLong;
		bwdref = bwdRefLong;
	}
	else{
		fwdref = fwdRefShort;
		bwdref = bwdRefShort;
	}
    if ((sensorDifference > fwdref)||(sensorDifference < bwdref)){
		if(movedToNewPosition < 2){
			movedToNewPosition++;
			return;
		}
		else{
			movedToNewPosition = 0;
			
			moveRobot();
		}
	}
	else if (movedToNewPosition == 1)
	{
		movedToNewPosition = 0;
	}
	//backToStart(); // not tested fully, could still give nonsense.
}

void Robot::moveRobot(){
    
    if (haltAfterSection) {
        this->setSpeed(0);
        drive();
    }
	
	commObj->reactivateRFID(); // this cannot be called upon from within a interrupt 
	MapSection* tempSection;
	
	switch (direction)
	{
		//-------------------------Direction is forwards in map-------------------
		case 'f':
		//save section about to move into to temp container
		tempSection = mom->getPos(xCoord,yCoord+1);
		//move robot to new section
		mom->setSection(xCoord,yCoord+1,this);
		//put previousSection back to last position.
		mom->setSection(xCoord,yCoord,previousSection);
		//save temp section to previous section
		previousSection = tempSection;
		
		yCoord++;
		break;
		
		//-------------------------Direction is backwards in map-------------------
		case 'b':
		//save section about to move into to temp container
		tempSection = mom->getPos(xCoord,yCoord-1);
		//move robot to new section
		mom->setSection(xCoord,yCoord-1,this);
		//put previousSection back to last position.
		mom->setSection(xCoord,yCoord,previousSection);
		//save temp section to previous section
		previousSection = tempSection;
		
		yCoord--;
		
		break;
		
		//-------------------------Direction is right in map-----------------------
		case 'r':
		//save section about to move into to temp container
		tempSection = mom->getPos(xCoord-1,yCoord);
		//move robot to new section
		mom->setSection(xCoord-1,yCoord,this);
		//put previousSection back to last position.
		mom->setSection(xCoord,yCoord,previousSection);
		//save temp section to previous section
		previousSection = tempSection;
		
		xCoord--;
		
		break;
		
		//-------------------------Direction is left in map------------------------
		case 'l':
		//save section about to move into to temp container
		tempSection = mom->getPos(xCoord+1,yCoord);
		//move robot to new section
		mom->setSection(xCoord+1,yCoord,this);
		//put previousSection back to last position.
		mom->setSection(xCoord,yCoord,previousSection);
		//save temp section to previous section
		previousSection = tempSection;
		
		xCoord++;
		
		break;
		
		//-------------------------Direction is undefined.-------------------------
		default :
		//would like to throw some kind of error here.
		return;
	}
	setFwdClosed();
	setBwdClosed();
	setRightClosed();
	setLeftClosed();
	validSensor = determineValidSensor();
	if (validSensor == 'w'){
		commObj->activateWheelSensor();
	}
	if(validSensor == 'f'){
		this->setFwdReference();
	}
	else if(validSensor == 'b'){
		this->setBwdReference();
	}
	else{
		validSensor = 'N';
		this->setBwdReference();
		this->setFwdReference();
	}
}



// ------------Move Functions------------------


void Robot::moveForward(){
    MapSection* tempSection;
    
    tempSection = mom->getPos(xCoord,yCoord+1);
    //move robot to new section
    mom->setSection(xCoord,yCoord+1,this);
    //put previousSection back to last position.
    mom->setSection(xCoord,yCoord,previousSection);
    //save temp section to previous section
    previousSection = tempSection;
    
    yCoord++;
    
}

void Robot::moveBackward(){
    
    MapSection* tempSection;
    //save section about to move into to temp container
    tempSection = mom->getPos(xCoord,yCoord-1);
    //move robot to new section
    mom->setSection(xCoord,yCoord-1,this);
    //put previousSection back to last position.
    mom->setSection(xCoord,yCoord,previousSection);
    //save temp section to previous section
    previousSection = tempSection;
    
    yCoord--;
    
}


void Robot::moveRight(){
    
    MapSection* tempSection;
    //save section about to move into to temp container
    tempSection = mom->getPos(xCoord-1,yCoord);
    //move robot to new section
    mom->setSection(xCoord-1,yCoord,this);
    //put previousSection back to last position.
    mom->setSection(xCoord,yCoord,previousSection);
    //save temp section to previous section
    previousSection = tempSection;
    
    xCoord--;
    
}

void Robot::moveLeft(){
    
    MapSection* tempSection;
    //save section about to move into to temp container
    tempSection = mom->getPos(xCoord+1,yCoord);
    //move robot to new section
    mom->setSection(xCoord+1,yCoord,this);
    //put previousSection back to last position.
    mom->setSection(xCoord,yCoord,previousSection);
    //save temp section to previous section
    previousSection = tempSection;
    
    xCoord++;
    
}


// -----------------------------------------

char Robot::determineValidSensor(){
	if((getBwdDistance()>0) & (getFwdDistance()>0)){
		//commObj->activateWheelSensor(); // this might not be able to be called upon from within an interrupt 
		return 'w';
	}
    else if( getFwdDistance() > getBwdDistance()){ // bwd sensor is smaller than fwd.
        return 'b';
    }
    else{                   //fwd sensor is smaller than bwd.
        return 'f';         //Doesnt this case happen alot more now that we only have a short sensor in the front?
    }
}

//------------------------------------------



void Robot::adjustPosition(){
	volatile int pd = 0;
	volatile int frontError = 0;
	volatile int backError = 0;
	volatile int deltaFrontError = 0;
	volatile int deltaBackError = 0;
	volatile int kp2 = 4;
	
	
	//front menar h�gerfram, back h�ger bak
	
    frontError=Ref-rightFrontSensor;
	backError=Ref-rightBackSensor;
	
	
	//skapar differans som betraktas som derivata
	deltaFrontError = frontError - previousFrontError;
	deltaBackError = backError - previousBackError;
	
	//nuvarande fel sparas som f�reg�ende. Notera att f�rsta cykeln ger h�g diff d� previousError initieras till noll.
	previousFrontError = frontError;
	previousBackError = backError;
	
	
	
	//B�da sensorerna ska reglera hjulparen p� samma vis, s�ledes kan vi addera ihop parametrarna och relgera samma hjulpar	
	pd = Kp*((frontError + backError)/2) + Kd*((deltaFrontError + deltaBackError)/2) - kp2(rightFrontSensor - rightBackSensor);
    
	turn(pd);
    
}

// ---------------------------------------
// Sets direction

void Robot::changeDirection(char inDirection){
    direction = inDirection;
}

// ----------------------------------------
// Get column from Map and send to Comm

char* Robot::getColAsChar(int col){
	asm("");
    return mom->getColAsChar(col);
}


// ----------------------------------------
int Robot::getFwdDistance(){
	usingLong = false;
	return fwdShortSensor;
}

int Robot::getBwdDistance(){
	if(bwdShortSensor < 50){
		asm("");
		usingLong = false;
		return bwdShortSensor;
	}
	else{
		asm("");
		usingLong = true;
		return bwdLongSensor;
	}
}

int Robot::getRightDistance(){
    
    int output = rightFrontSensor;
    output = output + rightBackSensor;
    return output/2;
    
}

int Robot::getLeftDistance(){
	
	if (leftMidSensor < 40){
		return 10;
	}
	else {
		return leftMidSensor;
	}
}

void Robot::setControlParameters(double inputKp, double inputKd, int inputRef, int inTrimLeft, int inTrimRight, int inFwdRefLong, int inBwdRefLong, int inFwdRefShort, int inBwdRefShort, int inRightCornerFront, int inRightCornerBack, int inRightWallFront, int inRightWallBack, int inHaltAfterSection){
    Kp=inputKp;
    Kd=inputKd;
    
    Ref=inputRef;
	
	fwdRefLong = inFwdRefLong;
	bwdRefLong = inBwdRefLong;
	fwdRefShort = inFwdRefShort;
	bwdRefShort = inBwdRefShort;
	
	rightCornerFront = inRightCornerFront;
	rightCornerBack = inRightCornerBack;
	rightWallFront = inRightWallFront;
	rightWallBack = inRightWallBack;
	
	haltAfterSection = (bool)inHaltAfterSection;
	
	// Trimming wall param
    
    this->trimLeft = inTrimLeft;
    this->trimRight = inTrimRight;
}

void Robot::setSpeed(int inSpeed)
{
	speed = inSpeed;
	movementSpeed=inSpeed;
}

// ------------------------ 
// Sets reference for mapping

void Robot::setFwdReference(){
	fwdReference = getFwdDistance();
}

void Robot::setBwdReference(){
    bwdReference = getBwdDistance();
}

void Robot::setUserSpeed(int inSpeed)
{
	speed = inSpeed;
	userSpeed = inSpeed;
	movementSpeed = inSpeed;
}

// --------------------------

// -----------------------

bool Robot::isCornerPassed(){
	if (rightBackSensor > 30)
	{
	return true;
	}
	return false;
}

bool Robot::isWallRight(){
	
    if ( (rightFrontSensor < rightWallFront && rightBackSensor < rightWallBack) ){
		volatile bool benny = true;
        return benny;
    }
    else {
		volatile bool benny = false;
        return benny;
    }
}

// --------------------------

bool Robot::isCornerRight(){
	
    if ( rightFrontSensor > rightCornerFront && rightBackSensor < rightCornerBack ){
		volatile bool shitFace = true;
        return shitFace;
    }
    else {
        return false;
    }
}

// --------------------------

bool Robot::isWallFwd(){
    
    if ( getFwdDistance() == 0 ) {
        return false;
    }
    if ( getFwdDistance() < 40 ){
        return true;
    }
    else{
        return false;
    }
    
}

// --------------------------

int Robot::getRightDifference(){
    int front;
    int back;
    front = rightFrontSensor;
    back = rightBackSensor;
    return front - back;
}


// ---------------------------

int Robot::getUserSpeed(){
    return userSpeed;
}

bool Robot::isWallFwdClose()
{
	    if ( getFwdDistance() == 0 ) {
		    return false;
	    }
	    if ( getFwdDistance() < 30 ){
		    return true;
	    }
	    else{
		    return false;
	    }
}

// ----------------

void Robot::robotRotated(){
	validSensor = determineValidSensor();
	if(validSensor == 'f'){
		this->setFwdReference();
	}
	else if(validSensor == 'b'){
		this->setBwdReference();
	}
	else{
		validSensor = 'N';
		this->setBwdReference();
		this->setFwdReference();
	}
	setFwdClosed();
	setBwdClosed();
	setRightClosed();
	setLeftClosed();
}

//-----------------

void Robot::setRotateRightActive()
{
	
	rotateRightActive = true;
	rotateActive = true;
}

// -----------------

bool Robot::getRotateRightActive()
{
	return rotateRightActive;
}

// ------------------

void Robot::setRotateLeftActive()
{
	rotateLeftActive = true;
	rotateActive = true;
}

// ------------------

bool Robot::getRotateLeftActive()
{
	return rotateLeftActive;
}

/*
 *  Wait until two new sensor arrays have been delivered. Two so that we are sure 
 *  that we have good values on the steermodule for decision making. 
 */

void Robot::waitForNewData()
{
    for (unsigned int i = 0; i < 2; i++) {
        newData = false;
        while(!newData){
            asm("");
            volatile int p = 0;
            p++;
        }
    }
}

void Robot::backToStart()
{
	//detta b�r kontrolleras mycket mera
	if((previousSection->getX() == 16) &&	(previousSection->getY()==1)){
        if ( mom->getPos(xCoord,yCoord - 1)->isClosed(xCoord,yCoord - 1, 0) ){
            mom->fillClosedArea();
            
            startExplore = true;
        }
	}	
}

// ----------------------

