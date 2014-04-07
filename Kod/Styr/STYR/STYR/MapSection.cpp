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
// All we need to do is change type.

Robot::Robot(int xPos, int yPos, Map* inMom) : MapSection(xPos, yPos, inMom){
	type = 'r';
	direction = 'f';
}

// -------------------------------------
// Sets direction to travel

void Robot::changeDirection(char direction){
		if (direction == 'b'){
            
            #ifndef DEBUG
			PORTD |= 0x10;
			PORTD &= ~0x20;
            #endif
		}
		else if (direction == 'r'){
            
            #ifndef DEBUG
			PORTD &= ~0x10;
			PORTD |= 0x20;
            #endif
		}
		else if (direction == 'l'){
            
            #ifndef DEBUG
			PORTD |= 0x10;
			PORTD |= 0x20;
            #endif
		}
		else if (direction == 'f'){
            
            #ifndef DEBUG
			PORTD &= ~0x10;
			PORTD &= ~0x20;
            #endif
		}
}

// ------------------------------------
// Drives

void Robot::drive(int speed){
	int output = floor(speed * 255 / 100);
	
	#ifndef DEBUG
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

// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdValueIn(int fwd){
	// Determine how far we've traveled since last read
	int diff = fwd - fwdSensor[0];
	fwdDiff = fwdDiff + diff;
	fwdSensor = pushBackInt(fwdSensor, fwd);
	
	// If next to a wall - stop!
	if (fwd < 10)
	{
		drive(0);
		rotateLeft();
	}
}

void Robot::bwdValueIn(int bwd){
	bwdSensor = pushBackInt(bwdSensor, bwd);
}

void Robot::leftValueIn(int left){
	leftSensor = pushBackInt(leftSensor, left);
}

void Robot::rightValueIn(int right){
	rightSensor = pushBackInt(rightSensor, right);
}

void Robot::phiDotValueIn(int phiDot){
	phiDotSensor = pushBackInt(phiDotSensor, phiDot);
}

// --------------------------------------------
// Sets walls in Map

void Robot::setFwdClosed(){
	
	// Create reasonable valid data from latest reads.
	int total = 0;
	int iterations = 3;
	
	for (int i = 0; i < iterations; i++){
		total = total + fwdSensor[i];
	}
	
	int mean = total / iterations;
	// A block is 40x40
	int output = mean / 40;
	
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

// ----------

void Robot::setBwdClosed(){
	
	// Create reasonable valid data from latest reads.
	int total = 0;
	int iterations = 3;
	
	for (int i = 0; i < iterations; i++){
		total = total + fwdSensor[i];
	}
	
	int mean = total / iterations;
	// A block is 40x40
	int output = mean / 40;
	
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