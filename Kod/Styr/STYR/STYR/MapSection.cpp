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
	return xCoord;
}

int MapSection::getY(){
	return yCoord;
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

Robot::Robot(int xPos, int yPos, Map* inMom) : MapSection(xPos, yPos, inMom){
	type = 'r';
	direction = 'f';
    
    mom->setSection(xPos,yPos,this);
}

Robot::~Robot(){
    mom->setSection(xCoord,yCoord,new MapSection(xCoord,yCoord,mom));
}

// -------------------------------------
// Sets direction to travel

void Robot::changeDirection(char direction){
		if (direction == 'b'){
            
            #if DEBUG == 0
			PORTD |= 0x10;
			PORTD &= ~0x20;
            #endif
		}
		else if (direction == 'r'){
            
            #if DEBUG == 0
			PORTD &= ~0x10;
			PORTD |= 0x20;
            #endif
		}
		else if (direction == 'l'){
            
            #if DEBUG == 0
			PORTD |= 0x10;
			PORTD |= 0x20;
            #endif
		}
		else if (direction == 'f'){
            
            #if DEBUG == 0
			PORTD &= ~0x10;
			PORTD &= ~0x20;
            #endif
		}
}

// ------------------------------------
// Drives

void Robot::drive(int speed){
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
	rotateActive = true;
	// Seft diffs to 0
	fwdDiff = 0;
	bwdDiff = 0;
	//---------
	// First send stuff to sensor module
	// When we have rotated 90 degrees sensor module will send a signal which will deactivate rotate
	//---------
	
	// Turns
	while (rotateActive)
	{
		changeDirection('l');
		drive(25);
	}
	
	// Stop rotation and set gear to forward
	drive(0);
	changeDirection('f');
}

// Stops rotation
void Robot::stopRotation(){
    rotateActive = false;
}

// ------------------------------------

void Robot::rotateRight(){
	// Rotate mode
	rotateActive = true;
	// Seft diffs to 0
	fwdDiff = 0;
	bwdDiff = 0;
	//---------
	// First send stuff to sensor module
	// When we have rotated 90 degrees sensor module will send a signal which will deactivate rotate
	//---------
	
	// Turns
	while (rotateActive)
	{
		changeDirection('r');
		drive(25);
	}
	
	// Stop rotation and set gear to forward
	drive(0);
	changeDirection('f');
}

//-----------------------------------------

void Robot::turnLeft(int speed){
    int output = floor(speed * 255 / 100);
	
#if DEBUG == 0
    OCR2A = output/2;
    OCR2B = output;
#endif
    
}

void Robot::turnRight(int speed){
    int output = floor(speed * 255 / 100);
	
#if DEBUG == 0
    OCR2A = output;
    OCR2B = output/2;
#endif
	
}

// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdValueIn(char* fwd){
	
    for (int i = 0; i < 3; i++) {
        pushBackChar(fwdSensor, fwd[i]);
    }
}

void Robot::bwdValueIn(char* bwd){
	for (int i = 0; i < 3; i++) {
        pushBackChar(bwdSensor, bwd[i]);
    }
}

void Robot::leftValueIn(char* left){
	for (int i = 0; i < 3; i++) {
        pushBackChar(leftSensor, left[i]);
    }
}

void Robot::rightValueIn(char* right){
	for (int i = 0; i < 3; i++) {
        pushBackChar(rightSensor, right[i]);
    }
}

void Robot::phiDotValueIn(char* phiDot){
	for (int i = 0; i < 3; i++) {
        pushBackChar(phiDotSensor, phiDot[i]);
    }
}

// --------------------------------------------
// Sets walls in Map

void Robot::setFwdClosed(){
	
	int output = meanValueArray(fwdSensor,3)/40;
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		mom->convertSection(xCoord,yCoord + output + 1, 'c');
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		mom->convertSection(xCoord,yCoord - output - 1, 'c');
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		mom->convertSection(xCoord + output + 1,yCoord, 'c');
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		mom->convertSection(xCoord - output - 1,yCoord, 'c');
	}
	
}

// ------------------------------------------------

void Robot::setBwdClosed(){
	
	// A block is 40x40
	int output = meanValueArray(bwdSensor,3)/40;
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		mom->convertSection(xCoord,yCoord - output - 1, 'c');
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		mom->convertSection(xCoord,yCoord + output + 1, 'c');
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		mom->convertSection(xCoord - output - 1,yCoord, 'c');
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		mom->convertSection(xCoord + output + 1,yCoord, 'c');
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
    if (meanValueArray(fwdSensor,3) > 340 && meanValueArray(bwdSensor,3) > 340) {
        fwdReference = 300;
        bwdReference = 300;
    }
    if (fwdReference-meanValueArray(fwdSensor,3)<=40){
        fwdReference=meanValueArray(fwdSensor,3);
        if (direction == 'f'){
            mom->setSection(xCoord,yCoord + 1, this);
        }
        else if (direction == 'b'){
            mom->setSection(xCoord,yCoord - 1, this);
        }
        else if (direction == 'r'){
            mom->setSection(xCoord + 1,yCoord, this);
        }
        else if (direction == 'l'){
            mom->setSection(xCoord - 1,yCoord, this);
        }
    }
    else if (meanValueArray(bwdSensor,3)-bwdReference <=40){
        bwdReference=meanValueArray(bwdSensor,3);
        if (direction == 'f'){
            mom->setSection(xCoord,yCoord + 1, this);
        }
        else if (direction == 'b'){
            mom->setSection(xCoord,yCoord - 1, this);
        }
        else if (direction == 'r'){
            mom->setSection(xCoord + 1,yCoord, this);
        }
        else if (direction == 'l'){
            mom->setSection(xCoord - 1,yCoord, this);
        }

    }
     

}


// -----------------------------------------

void Robot::adjustPosition(){
    if (meanValueArray(leftSensor,3)>90) {
        leftReference=80;
    }
    
    // undefined sensorvalues will be set to 80//
    
    else if (meanValueArray(rightSensor,3)>90) {
        rightReference=80;
    }
    else{
        rightReference = meanValueArray(rightSensor,3);
        leftReference = meanValueArray(leftSensor,3);
        //Changes the references on the side sensors to help position adjustment
    }
    
    while (rightReference<15){
        turnLeft(25);
        rightReference = meanValueArray(rightSensor,3);
        //Not sure if new sensor values will be acquired during loop.
        
    }
    
    while (leftReference<15){
        turnRight(25);
        leftReference = meanValueArray(leftSensor,3);
    }
    
    
}

