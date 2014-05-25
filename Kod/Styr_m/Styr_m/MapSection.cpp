#include "./MapSection.h"
#include "./Map.h"

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

// ----------------------------
// Set coordinates and map segment type.

void MapSection::setX(int coord){
	xCoord=coord;
}

void MapSection::setY(int coord){
	yCoord=coord;
}

void MapSection::setType(char newType){
	type=newType;
}


//---------------------------------------------------------
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
// Retrieves coordinates of different sections relative to asked section.

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
// Check to see if the asked section is of type "unexplored"

bool MapSection::isUnexplored(){
	if (type == 'u') {
		return true;
	}
	else{
		return false;
	}
}

// --------------------------------------------------------------
// Cancer function for filling unexplored spaces not reachable

void MapSection::cancer(){
    this->setType('c');
    
    // 9 o clock
    if ( mom->withinMap(xCoord - 1,yCoord) && mom->getPos(xCoord - 1, yCoord)->getType() == 'u' ) {
        
        mom->getPos(xCoord - 1, yCoord)->cancer();
        
	}
    // Check 12
    if ( mom->withinMap(xCoord, yCoord - 1) && mom->getPos(xCoord, yCoord - 1)->getType() == 'u' ) {
        
        mom->getPos(xCoord, yCoord - 1)->cancer();
	}
    // Check 3
    if ( mom->withinMap(xCoord + 1, yCoord) && mom->getPos(xCoord + 1, yCoord)->getType() == 'u' ) {
        
        mom->getPos(xCoord + 1, yCoord)->cancer();
	}
    // Check 6
    if ( mom->withinMap(xCoord, yCoord + 1) && mom->getPos(xCoord, yCoord + 1)->getType() == 'u' ) {
        
        mom->getPos(xCoord, yCoord + 1)->cancer();
	}
}



/*	-------------------------------------------------------------
 
 ROBOT
 
 -----------------------------------------------------------*/

// Construct ---------------------------
// Since this is a subclass - the MapSection constructor runs first.
// All we need to do is change type and set ourselves on map

Robot::Robot(int xPos, int yPos, Map* inMom, Communication* inComm) : MapSection(xPos, yPos, inMom){
	type = 'r';
	direction = 'f';
	
	Kd = 7;
	Kp = 5;
	Ref = 12;
	
	trimRight = 15;
	trimLeft = 0;
	
	fwdRefLong = 38;
	bwdRefLong = 2;
	
	fwdRefShort = 22;
	bwdRefShort = 4;
	
	rotateRightActive = false;
	rotateLeftActive = false;
    
    commObj = inComm;
    previousSection = mom->getPos(xPos,yPos);
	previousSection->setType('e');
    previousSection->isVisited = true;
    mom->setSection(xPos,yPos,this);
}

Robot::~Robot(){
    mom->setSection(xCoord,yCoord,new MapSection(xCoord,yCoord,mom));
}

// -------------------------------------
// Sets direction to travel

void Robot::changeGear(char inGear){
    
    currentGear = inGear;
    if (inGear == 'b'){
        
        gear = inGear;
        
#if TESTING == 0
        PORTD &= ~(1<<PORTD4); //0x10;
        PORTD &= ~(1<<PORTD5); //0x20;
#endif
    }
    else if (inGear == 'r'){
        gear = inGear;
        
#if TESTING == 0
        PORTD &= ~(1<<PORTD5);
        PORTD |= (1<<PORTD4);
#endif
    }
    else if (inGear == 'l'){
        gear = inGear;
        
#if TESTING == 0
        PORTD &= ~(1<<PORTD4);
        PORTD |= (1<<PORTD5);
#endif
    }
    else if (inGear == 'f'){
        gear = inGear;
        
#if TESTING == 0
        PORTD |= (1<<PORTD4);
        PORTD |= (1<<PORTD5);
#endif
    }
}

// ------------------------------------
void Robot::setRFID(){
	previousSection->setType('f');
}


// Drives

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
#if TESTING == 0
		OCR2A = outputLeft;
		OCR2B = outputRight;
#endif
	}
	else {
#if TESTING == 0
		OCR2A = 0;
		OCR2B = 0;
#endif
	}
}

void Robot::driveBackward(int speed){
    changeGear('b');
	int output = floor(speed * 255 / 100);
	
#if TESTING == 0
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
	newData = false;
	// Seft diffs to 0
	fwdDiff = 0;
	bwdDiff = 0;
	
	//send map
	
	commObj->sendMap();
    // Send request to sensor module to measure angle
    commObj->sendRotateRequest();
	
    // Turns
    changeGear('l');
    setSpeed(35);
	
	while (rotateActive)
	{
		drive();
	}
	
	okayToClose = false;
	// Corr our position on turns
	updateRobotPosition();
	okayToClose = true;
	
	// Stop rotation and set gear to forward
	setSpeed(0);
	changeGear('f');
	drive();
	waitForNewData();
    
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
	waitForNewData();
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
	
	//send map
	
	commObj->sendMap();
	
	//---------
	// First send stuff to sensor module
	// When we have rotated 90 degrees sensor module will send a signal which will deactivate rotate
	//--------
	
	// Protect against missing corners
	if (rotationCount > 4)
	{
		setSpeed(25);
		drive();
		while (!wheelHasTurned)
		{
			asm("");
		}
		updateRobotPosition();
		return;
	}
	commObj->sendRotateRequest();
	rotationCount++;
	
    // Turns
    changeGear('r');
    setSpeed(35);
    newData = false;
	while (rotateActive)
	{
 		drive();
	}
	
	okayToClose = false;
	// Corr our position on turns
	updateRobotPosition();
	okayToClose = true;
	
	// Stop rotation and set gear to forward
	setSpeed(0);
	changeGear('f');
	drive();
    waitForNewData();
	
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
	waitForNewData();
	waitForNewData();
	this->robotRotated();
	
}

//-----------------------------------------
// Turns robot slowly left or right, decided by the adjustposition function.
// Sets different speeds on the DC-engines.

void Robot::turn(int pd){
    int output = floor(movementSpeed * 255 / 100);
	
	int pdOut = pd;
	
	// Protect against overflow
	if (output+pdOut > 160 || output-pdOut < 0)
	{
#if TESTING == 0
		OCR2A = output; //Negative value on pd will turn left, positive right
		OCR2B = output;
#endif
	}
	else {
#if TESTING == 0
		OCR2A = output+pdOut; //Negative value on pd will turn left, positive right
		OCR2B = output-pdOut;
#endif
	}
}


// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdLongValueIn(char fwd[3]){
    fwdLongSensor = 100 * fwd[0];
    fwdLongSensor += 10 * fwd[1];
    fwdLongSensor += fwd[2];
    
#if TESTING == 1
    cout << "fwdValueIn" << endl;
    cout << fwdLongSensor << endl;
#endif
}

void Robot::bwdLongValueIn(char* bwd){
    bwdLongSensor = 100 * bwd[0];
    bwdLongSensor += 10 * bwd[1];
    bwdLongSensor += bwd[2];
    
#if TESTING == 1
    cout << "bwdValueIn" << endl;
    cout << bwdLongSensor << endl;
#endif
}

void Robot::bwdShortValueIn(char bwdShort[3]){
    bwdShortSensor = 100 * bwdShort[0];
    bwdShortSensor += 10 * bwdShort[1];
    bwdShortSensor += bwdShort[2];
    
#if TESTING == 1
    cout << "bwdShortValueIn" << endl;
    cout << bwdShortSensor << endl;
#endif
}

void Robot::fwdShortValueIn(char fwdShort[3]){
    fwdShortSensor = 100 * fwdShort[0];
    fwdShortSensor += 10 * fwdShort[1];
    fwdShortSensor += fwdShort[2];
    
#if TESTING == 1
    cout << "fwdShortValueIn" << endl;
    cout << fwdShortSensor << endl;
#endif
}

void Robot::leftLongValueIn(char left[3]){
    leftMidSensor = 100 * left[0];
    leftMidSensor += 10 * left[1];
    leftMidSensor += left[2];
    
#if TESTING == 1
    cout << "leftLongValueIn" << endl;
    cout << leftMidSensor << endl;
#endif
}

void Robot::rightBackValueIn(char right[3]){
	rightBackSensor = 100 * right[0];
    rightBackSensor += 10 * right[1];
    rightBackSensor += right[2];
    
#if TESTING == 1
    cout << "rightBackValueIn" << endl;
    cout << rightBackSensor << endl;
#endif
}

void Robot::rightFrontValueIn(char right[3]){
    rightFrontSensor = 100 * right[0];
    rightFrontSensor += 10 * right[1];
    rightFrontSensor += right[2];
    
#if TESTING == 1
    cout << "rightFrontValueIn" << endl;
    cout << rightFrontSensor << endl;
#endif
}

// --------------------------------------------
// setClosed functions sets walls in Map.

void Robot::setFwdClosed(){
	
    int output = 0;
    if ( getFwdDistance() < 30 ) {
        output = 0;
    }
    else if ( getFwdDistance() < 70 ){
        output = 1;
    }
	else {
		output = 2;
	}
	
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
        
        // Set every section between robot and wall as empty
		for (int i = 0; i < output; i++) {
            if(yCoord+i+1>16){
                break;
            }
			if(mom->getPos(xCoord,yCoord + i + 1)->getType() != 'f' && mom->getPos(xCoord,yCoord + i + 1)->getType() != 'c'){
                mom->convertSection(xCoord,yCoord + i + 1, 'e');
			}
		}
    }
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-i-1<0){
                break;
            }
			if(mom->getPos(xCoord,yCoord - i - 1)->getType() != 'f' && mom->getPos(xCoord,yCoord - i - 1)->getType() != 'c'){
				mom->convertSection(xCoord,yCoord - i - 1, 'e');
			}
        }
	
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){									//left right kan vara omvänt, måste testas
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord+i+1>31){
                break;
            }
			if(mom->getPos(xCoord + i + 1,yCoord)->getType() != 'f' && mom->getPos(xCoord + i + 1,yCoord)->getType() != 'c'){
				mom->convertSection(xCoord + i + 1,yCoord, 'e');
			}
        }
	
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord-i-1<0){
                break;
            }
			if(mom->getPos(xCoord - i - 1,yCoord)->getType() != 'f' && mom->getPos(xCoord - i - 1,yCoord)->getType() != 'c'){
                mom->convertSection(xCoord - i - 1,yCoord, 'e');
			}
        
	}
}

// ------------------------------------------------

void Robot::setBwdClosed(){
	
	int output = 0;
	
	if(speed != 0 || gear != 'l'){
		return;
	}
	if (getBwdDistance() > 80) {
		output = 80/40;
	}
	else{
		output = getBwdDistance()/40;
	}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-1-i < 0){
                break;
            }
			if(mom->getPos(xCoord,yCoord - i - 1)->getType() != 'f' && mom->getPos(xCoord,yCoord - i - 1)->getType() != 'c'){
				mom->convertSection(xCoord,yCoord - i - 1, 'e');
			}
        }
		if ( output == 0 && speed == 0 && !mom->getVisited(xCoord,yCoord-1)) {
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
			if(mom->getPos(xCoord,yCoord + i + 1)->getType() != 'f' && mom->getPos(xCoord,yCoord + i + 1)->getType() != 'c'){
				mom->convertSection(xCoord,yCoord + i + 1, 'e');
			}
		}
		if ( output == 0 && speed == 0 && !mom->getVisited(xCoord,yCoord+1)) {
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
			if(mom->getPos(xCoord - i - 1,yCoord)->getType() != 'f' && mom->getPos(xCoord - i - 1,yCoord)->getType() != 'c'){
				mom->convertSection(xCoord - i - 1,yCoord, 'e');
			}
        }
		if ( output == 0 && speed == 0  && !mom->getVisited(xCoord-1,yCoord)) {
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
			if (mom->getPos(xCoord + i + 1,yCoord)->getType() != 'f' && mom->getPos(xCoord + i + 1,yCoord)->getType() != 'c'){
				mom->convertSection(xCoord + i + 1,yCoord, 'e');
			}
        }
		if ( output == 0 && speed == 0  && !mom->getVisited(xCoord+1,yCoord)) {
			mom->convertSection(xCoord + 1,yCoord, 'c');
		}
	}
}

// -------------- To the left --------------------------

void Robot::setLeftClosed(){
    if(!islandMode || exploringIsland){
	    return;
    }
	
	int output = 0;
    
    if(leftMidSensor < 40){
        output = 0;
    }
	else if(leftMidSensor > 39 && leftMidSensor < 60){
		output = 1;
	}
	else if( leftMidSensor > 79 && leftMidSensor < 100){
		output = 2;
	}
    else{
        return;
    }
    
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
        
        // Set every section between robot and wall as empty or closed
        for (int i = 0; i < output; i++) {
            if(xCoord+1+i > 31){
                break;
            }
            // Explored except if closed since short range sensors have higher priority
            if ( mom->getPos(xCoord + i + 1, yCoord)->getType() != 'c' && mom->getPos(xCoord + i + 1, yCoord)->getType() != 'f'){
                mom->convertSection(xCoord + i + 1,yCoord, 'e');
            }
        }
		if(output< 3 && !mom->getVisited(xCoord+1+output,yCoord) && mom->getPos(xCoord + output + 1, yCoord)->getType() != 'c'){
			mom->convertSection(xCoord + output + 1,yCoord, 'c');
			islandToLeft = true;
		}
	}
    
    
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
        
        // Set every section between robot and wall as empty or closed
        for (int i = 0; i < output; i++) {
            if(xCoord-1-i < 0){
                break;
            }
            // Explored except if closed since short range sensors have higher priority
            if ( mom->getPos(xCoord - i - 1, yCoord)->getType() != 'c' && mom->getPos(xCoord - i - 1, yCoord)->getType() != 'f' ){
                mom->convertSection(xCoord - i - 1,yCoord, 'e');
            }
        }
		if(output < 4 && !mom->getVisited(xCoord-1-output,yCoord) && mom->getPos(xCoord - output - 1, yCoord)->getType() != 'c'){
			mom->convertSection(xCoord - output - 1,yCoord, 'c');
			islandToLeft = true;
		}
	}
	// Direction 0->x->32, "right"
	else if (direction == 'l'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord-1-i < 0){
                break;
            }
            // Explored except if closed since short range sensors have higher priority
            if ( mom->getPos(xCoord, yCoord - i - 1)->getType() != 'c' && mom->getPos(xCoord, yCoord - i - 1)->getType() != 'f'){
                mom->convertSection(xCoord,yCoord - i - 1, 'e');
            }
        }
		if(output < 4  && !mom->getVisited(xCoord,yCoord-1-output) && mom->getPos(xCoord, yCoord - 1 - output)->getType() != 'c'){
			mom->convertSection(xCoord,yCoord - 1 - output, 'c');
			islandToLeft = true;
		}
	}
	// Direction 32->x->0, "left"
	else if (direction == 'r'){
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(yCoord+1+i > 16){
                break;
            }
            // Explored except if closed since short range sensors have higher priority
            if ( mom->getPos(xCoord, yCoord + i + 1)->getType() != 'c' && mom->getPos(xCoord, yCoord + i + 1)->getType() != 'f' ){
                mom->convertSection(xCoord,yCoord + i + 1, 'e');
            }
        }
		if(output < 4  && !mom->getVisited(xCoord,yCoord+1+output) && mom->getPos(xCoord, yCoord + output + 1)->getType() != 'c'){
			mom->convertSection(xCoord,yCoord + 1 + output, 'c');
			islandToLeft = true;
		}
	}
}

// -------------- To the right --------------------------

void Robot::setRightClosed(){
	
	int output = 0;
    
	
	if ( rightFrontSensor > 60 ) { // this value might need to be calibrated
		output = 80/40;//if distance is great only print max 2 empty.
	}
	else{
		output = rightFrontSensor/40;	//kanske borde ha +10 innan man delar med 40 för att robotens inte är längst till höger i rutan
	}
	if (output != 0){
		return;
	}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            if(xCoord-1-i < 0){
                break;
            }
            if ( mom->getPos(xCoord - i - 1, yCoord)->getType() != 'c' && mom->getPos(xCoord - i - 1, yCoord)->getType() != 'f' ) {
                mom->convertSection(xCoord - i - 1,yCoord, 'e');
            }
        }
        if(output == 0 && !mom->getVisited(xCoord-1,yCoord)){
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
            if ( mom->getPos(xCoord + i + 1, yCoord)->getType() != 'c' && mom->getPos(xCoord + i + 1, yCoord)->getType() != 'f' ) {
                mom->convertSection(xCoord + i + 1,yCoord, 'e');
            }
            
        }
        if(output == 0 && !mom->getVisited(xCoord+1,yCoord)){
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
            if ( mom->getPos(xCoord, yCoord + i + 1)->getType() != 'c' && mom->getPos(xCoord, yCoord + i + 1)->getType() != 'f' ) {
                mom->convertSection(xCoord,yCoord + i + 1, 'e');
            }
        }
        if(output == 0 && !mom->getVisited(xCoord,yCoord+1)){
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
            if ( mom->getPos(xCoord, yCoord - i - 1)->getType() != 'c' && mom->getPos(xCoord, yCoord - i - 1)->getType() != 'f' ) {
                mom->convertSection(xCoord,yCoord - i - 1, 'e');
            }
        }
        if(output == 0 && !mom->getVisited(xCoord,yCoord-1)){
            mom->convertSection(xCoord,yCoord - 1, 'c');
        }
	}
}

//------------------------------------------------------
// Checks to see if the robot has passed a corner.
bool Robot::isCornerPassed(){
	if (rightBackSensor > 25)
	{
		return true;
	}
	return false;
}

// -----------------------------------------
//Sets reference values and moves robot in map abstraction if robot has moved one square
void Robot::updateRobotPosition(){
    
    if (wheelHasTurned){
		//for debugging
        wheelHasTurned = false;
        commObj->reactivateWheelSensor();
        MapSection* tempSection;
        
        if(commObj->isRFID){
	        setRFID();
	        commObj->isRFID=false;
        }
        
		switch (direction){
                
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
                
                //put back last section and tell it, it has been visited.
                previousSection->isVisited = true;
				if (previousSection->getType() != 'f')
				{
					previousSection->setType('e');
				}
				
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
				
                previousSection->isVisited = true;
				if (previousSection->getType() != 'f')
				{
					previousSection->setType('e');
				}
                
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
				
                previousSection->isVisited = true;
				if (previousSection->getType() != 'f')
				{
					previousSection->setType('e');
				}
                
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
				
                previousSection->isVisited = true;
				if (previousSection->getType() != 'f')
				{
					previousSection->setType('e');
				}
				
				xCoord++;
                
				break;
                
                //-------------------------Direction is undefined.-------------------------
			default :
				//would like to throw some kind of error here.
				return;
		}
		if((RFIDmode)&&(rightFrontSensor < 20)&&(okayToClose)){
			setRightClosed();
		}
		else if (okayToClose && !RFIDmode){
			setFwdClosed();
			setBwdClosed();
			setRightClosed();
			setLeftClosed();
		}
		if(isHome()){
			setSpeed(0);
			drive();
            closeMap();
			islandMode = true;
        }
    }
}

// -----------------------------------------

char Robot::determineValidSensor(){
    
    if( getFwdDistance() >= getBwdDistance()){ // bwd sensor is smaller than fwd.
        return 'b';
    }
    else{                   //fwd sensor is smaller than bwd.
        return 'f';
    }
}

//------------------------------------------
// Control function. Compares reference value to
// actual sensor value to decide error.
// Also checks if robot is traveling straight ahead.
// Uses the turn function to adjust the robots position.



void Robot::adjustPosition(){
	volatile int pd = 0;
	volatile int frontError = 0;
	volatile int derivError = 0;
	volatile int backError = 0;
	volatile int error = 0;
	volatile int both = 0;
	
    frontError=Ref-rightFrontSensor;
	backError=Ref-rightBackSensor;
	
	both = frontError + backError;
	error = both/2;
	
    derivError=frontError - backError;
	pd= Kp*error + Kd*derivError;
    
	turn(pd);
    
}

// ---------------------------------------
// Sets direction

void Robot::changeDirection(char inDirection){
    direction = inDirection;
}

// ----------------------------------------
// Get column from Map and send to Comm

void Robot::getColAsChar(int col){
    // Sending 19 positions of interest
    colArray[0] = 23;
    // Sending Map data command
    colArray[1] = 'M';
    // Sending column number
    colArray[2] = col;
    
    for (int it = 0; it < 17; it++)
    {
        // Type of the block we are looking at
        colArray[it+3] = mom->getPos(col,it)->getType();
    }
}



// ----------------------------------------
int Robot::getFwdDistance(){
	return fwdShortSensor;
}

int Robot::getBwdDistance(){
	return bwdShortSensor;
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


//Changes the control parameters to the values set in GUI.
void Robot::setControlParameters(double inputKp, double inputKd, int inputRef, int trimLeft, int trimRight){
    Kp=inputKp;
    Kd=inputKd;
    
    Ref=inputRef;
    
    this->trimLeft = trimLeft;
    this->trimRight = trimRight;
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


// ---------------------------------------------------------
//Checks to see if there is a wall to the right of the robot.

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

// -------------------------------------------------------
// Checks if there is a corner to the right.

bool Robot::isCornerRight(){
	
    if ( rightFrontSensor > rightCornerFront && rightBackSensor < rightCornerBack ){
		volatile bool stuff = true;
        return stuff;
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
    if ( getFwdDistance() < 35 ){
        return true;
    }
    else{
        return false;
    }
    
}

// ----------------------------------------------------------
//Returns the difference between the sensors on the right side

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
    if ( getFwdDistance() < 20 ){
        return true;
    }
    else{
        return false;
    }
}

// -------------------------------------------------
// Function that closes sections depending on what
// situation and mode that is active

void Robot::robotRotated(){
	if((RFIDmode)&&(getBwdDistance() < 30)&&(okayToClose)){
		setBwdClosed();
	}
	if((RFIDmode)&&(rightFrontSensor < 20)){
		setRightClosed();
	}
	else if(!RFIDmode){
		setFwdClosed();
		setBwdClosed();
		setRightClosed();
		setLeftClosed();
	}
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

// --------------------------------
// Function used as a delay in main loop
// so that robot won't do anything until sensor
// data has been updated

void Robot::waitForNewData()
{
#if TESTING == 0
	asm("");
	volatile int temp = bwdShortSensor;
	asm("");
	_delay_ms(350);
	volatile int p;
	p++;
	_delay_ms(350);
	volatile int temp2 = bwdShortSensor;
	asm("");
	p++;
	p++;
	asm("");
#endif
}

// ----------------------------------------
// Used at the end of mapping to close the entire map if possible.

void Robot::closeMap()
{
    setSpeed(0);
    drive();
	// Activate explorer
    startExplore = true;
    // Cancer all u:s outside of closed area
    for (int x = 0; x < 32; x=x+31) { //first and last col
        for (int y = 0; y < 17; y++) { //All rows
            if ( mom->getPos(x,y)->getType() == 'u' ) {
                mom->getPos(x,y)->cancer();
            }
        }
    }
}

//-----------------------------------------------------
// Checks if robot has returned to start position.
bool Robot::isHome(){
    if(xCoord == 16 &&  yCoord==1 && direction == 'r'){
        return true;
    }else{
        return false;
    }
}

// Exploring
// -----------------------

void Robot::findFinishPos(){
    for (int i = 0; i < 32; i++) {
        for (int it = 0; it < 17; i++) {
            if ( mom->getPos(i,it)->getType() == 'u' ) {
                finishX = i;
                finishY = it;
                break;
            }
        }
    }
}

int Robot::getFinishX(){
    return finishX;
}

int Robot::getFinishY(){
	return finishY;
}

// -------------------------------------------
// Algorithm for robot behaviour at an "island".
// Island is a part of the map that is in the middle of an empty area.


void Robot::handleIsland()
{
	rotateLeft();
	if(isCornerRight()){
		while (!isCornerPassed()) {
			changeGear('f');
			setSpeed(15);
			drive();
        }
        //_delay_ms(25); // This delay ensures that we enter next segment.
        rotateRight();
		while (isCornerPassed()) {
            changeGear('f');
            setSpeed(15);
            drive();
        }
	}
	if(isWallFwd()){
		setSpeed(20);
		changeGear('f');
		while (!isWallFwdClose()){
			drive();
        }
        setSpeed(0);
        drive();
        if(!isWallRight()){
            rotateRight();
            //Drive forward untill robot has entered
            while (!isWallRight()) {
                changeGear('f');
                setSpeed(25);
                drive();
            }
        }
        else{
            rotateLeft();
        }
	}
	else{
		if(!isWallRight()){
			rotateRight();
		}
		else{
			setSpeed(25);
			changeGear('f');
			drive();
			adjustPosition();
        }
	}
}

int Robot::getFrontRightDistance()
{
	return rightFrontSensor;
}


void Robot::explore(){
    
    while(stillUnexplored()){
        if(lookForULeft()){
            goAcross();
            exploreX = xCoord;
            exploreY = yCoord;
        }else{
            while(!wheelHasTurned){
                followRight(); // follow right untill in new section.
            }
            updateRobotPosition();
            setSpeed(0);
            drive();
            if(xCoord == exploreX && yCoord == exploreY){
                goAcross();
            }
        }
    }
}

//---------------------------------------------
// Algorithm for exploring an "island", covers all
// the different situations that can occur.

void Robot::exploreIsland(){
	if(!islandToLeft && !exploringIsland){
		while(!wheelHasTurned){
			followRight();
			if(commObj->sendMapNow){
				asm("");
				commObj->sendMapNow=false;
				commObj->sendMap();
				asm("");
			}
		}
		
	}
	updateRobotPosition();
	if(islandToLeft && !exploringIsland){
		exploringIsland = true;
		goAcross();
		rotateLeft();
		savePosition();
		islandToLeft = false;
		timesMovedOnIsland = 0;
	}
	
	else if(exploringIsland){
		while(!wheelHasTurned){
			followRight();
		}
		updateRobotPosition();
		
		if(xCoord == islandStartX && yCoord == islandStartY && (timesMovedOnIsland != 0)){
			goAcross();
			rotateLeft();
			exploringIsland = false;
			islandToLeft = false;
		}
		timesMovedOnIsland++;
	}
}

bool Robot::lookForULeft(){
    switch(direction){
        case 'b' :
            for(int i = xCoord ; xCoord > 0; i--){
                if(mom->getPos(i,yCoord)->getType() == 'u'){
                    if(mom->getPos(i,yCoord)->detected > 2){
                        mom->getPos(i,yCoord)->setType('c');
                    }
                    else{
                        mom->getPos(i,yCoord)->detected++;
                        return true;
                    }
                }
            }
            return false;
            //---------------------------------------------------------
        case 'f' :
            
            for(int i = xCoord ; xCoord < 32; i++){
                if(mom->getPos(i,yCoord)->getType() == 'u'){
                    if(mom->getPos(i,yCoord)->detected > 2){
                        mom->getPos(i,yCoord)->setType('c');
                    }
                    else{
                        mom->getPos(i,yCoord)->detected++;
                        return true;
                    }
                }
            }
            return false;
            
            //---------------------------------------------------------
        case 'r' :
            
            for(int i = yCoord ; yCoord < 17; i++){
                if(mom->getPos(xCoord,i)->detected > 2){
                    mom->getPos(xCoord,i)->setType('c');
                }
                else{
                    mom->getPos(xCoord,i)->detected++;
                    return true;
                }
            }
            return false;
            
            //---------------------------------------------------------
        case 'l' :
            
            for(int i = yCoord ; yCoord > 0; i--){
                if(mom->getPos(xCoord,i)->detected > 2){
                    mom->getPos(xCoord,i)->setType('c');
                }
                else{
                    mom->getPos(xCoord,i)->detected++;
                    return true;
                }
            }
            return false;
    }
}

//-------------------------------------
//Positions robot for driving across an open area
// and drives untill a wall has been reached.

void Robot::goAcross(){
    rotateLeft();
    
    setSpeed(25);
    changeGear('f');
	drive();
    while(!isWallFwdExplore()){
        updateRobotPosition();
        
    }
}

//-------------------------------------------
// Algorithm for following a wall on the right
// side of the robot.
// Covers many different cases and situations that can occur.

void Robot::followRight(){
    
    if(isCornerRight()){
		
       while ( !isCornerPassed() && !(commObj->getManual())) {
            changeGear('f');
            setSpeed(20);
            drive();
            if(isCornerPassed()){
	            _delay_ms(200);
				setSpeed(0);
				drive();
				break;
            }
       }
        // This delay ensures that we enter next segment.
        rotateRight();
        
		this->changeGear('f');
		this->setSpeed(20);
		
		while (isCornerPassed() && !(commObj->getManual())) {
         this->drive();
        }
    }
    
    if(!isCornerRight()&&isWallFwd()){
        this->setSpeed(25);
        this->changeGear('f');
		this->drive();
        while (!this->isWallFwdClose() && !(commObj->getManual()))
        {
         asm("");
		 asm("");  
        }
        this->setSpeed(0);
        this->drive();
        
        
        if(!this->isWallRight())
        {
            this->rotateRight();
            //Drive forward untill robot has entered
             this->changeGear('f');
             this->setSpeed(25);
             this->drive();
			
			while (!this->isWallRight() && !(commObj->getManual())) {
               asm("");
               asm("");
            }
        }
        
        else
        {
            this->rotateLeft();
        }
        
    }
    else if(!isCornerRight())
    {
        if(!this->isWallRight())
        {
            this->rotateRight();
        }
        else
        {
            
            this->setSpeed(25);
            this->changeGear('f');
            this->drive();
            this->adjustPosition();
        }
    }
    
}



//------------------------------------
// Returns true if there are any sections that
// are unexplored
bool Robot::stillUnexplored(){
    for(int x = 0; x <32; x++){
        for(int y = 0; y <17;y++){
            if(mom->getPos(x,y)->getType() == 'u'){
                return true;
            }
        }
    }
    return false;
}

//--------------------------------------------------------
// Runs until robot has returned to starting position.

void Robot::goHome(){
    exploreX = xCoord;
    exploreY = yCoord;
    while(!isHome()){
        while(!wheelHasTurned){
            followRight(); // follow right untill in new section.
        }
        updateRobotPosition();
        if(exploreX == xCoord && exploreY == yCoord){
            goAcross();
            exploreX = xCoord;
            exploreY = yCoord;
        }
    }
	setSpeed(0);
	drive();
}


//----------------------------------------------------------
// Saves robot position, helps in island exploring.
// Robot will know when to return to outer wall.

void Robot::savePosition(){
	islandStartX = xCoord;
	islandStartY = yCoord;
}

//--------------------------------------------------------------
// Checks distance to wall at front of robot.
bool Robot::isWallFwdExplore()
{
	 if ( getFwdDistance() == 0 ) {
		 return false;
	 }
	 if ( getFwdDistance() < 25 ){
		 return true;
	 }
	 else{
		 return false;
	 }
}

