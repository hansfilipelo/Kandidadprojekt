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

// ----------------------------
// Set stuff

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

bool MapSection::isUnexplored(){
	if (type == 'u') {
		return true;
	}
	else{
		return false;
	}
}

// SLAM algorithms -------------------------------------

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


/*	-------------------------------------------------------------

						ROBOT
					
	-----------------------------------------------------------*/

// Construct ---------------------------
// Since this is a subclass - the MapSection constructor runs first.
// All we need to do is change type and set ourselves on map

Robot::Robot(int xPos, int yPos, Map* inMom, Communication* inComm) : MapSection(xPos, yPos, inMom){
	type = 'r';
	direction = 'f';
	
	Kd = 26;
	Kp = 7;
	Ref = 8;
	
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
	newData = false;
	// Seft diffs to 0
	fwdDiff = 0;
	bwdDiff = 0;
	
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
	
	while(!newData){
		
	}
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
	//---------
	// First send stuff to sensor module
	// When we have rotated 90 degrees sensor module will send a signal which will deactivate rotate
	//---------
	commObj->sendRotateRequest();
	
    // Turns
    changeGear('r');
    setSpeed(35);
    newData = false;
	while (rotateActive)
	{
 		drive();
	}
	
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
	
	while(!newData){
		asm("");
	}
	this->robotRotated();
	
}

//-----------------------------------------

void Robot::turn(int pd){
    int output = floor(movementSpeed * 255 / 100);
	
	int pdOut = pd * movementSpeed * 0.01;
	
	#if DEBUG == 0
	OCR2A = output+pdOut; //Negative value on pd will turn left, positive right
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
	else if (direction == 'l'){									//left right kan vara omvänt, måste testas
        
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
    if(validSensor == 'N'){
        validSensor = determineValidSensor();
    }
    int sensorDifference = 0;
    
    if (validSensor == 'b'){
		int ref = bwdReference/40;
        sensorDifference = getBwdDistance() - ref*40;
    }
    else if(validSensor == 'f'){
		int ref = fwdReference/40;
		sensorDifference = getFwdDistance() - ref*40;
    }
	
	/* The paramaters for sensor differences (references?) are called:
	fwdRefLong;
	bwdRefLong;
	fwdRefShort;
	bwdRefShort;
	*/
    
    if ((sensorDifference > 36)||(sensorDifference < -36)){
		commObj->reactivateRFID();	
		MapSection* tempSection;
		
		if(haltAfterSection){
			setUserSpeed(0);
			drive();
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
		//update which sensor that is valid and should be measured.
		//and update the references on that sensor.
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

char* Robot::getColAsChar(int col){
	asm("");
    return mom->getColAsChar(col);
}


// ----------------------------------------
int Robot::getFwdDistance(){
	if(fwdShortSensor < 80){
		return fwdShortSensor;
	}
	else{
		return fwdLongSensor;	
	}
}

int Robot::getBwdDistance(){
	if(bwdShortSensor < 80){
		return bwdShortSensor;
	}
	else{
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

// --------------------------

// -----------------------

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
    if ( getFwdDistance() < 35 ){
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
	    if ( getFwdDistance() < 20 ){
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

// ----------------------

void Robot::waitForNewData()
{
	_delay_ms(700);
}

// ----------------------


