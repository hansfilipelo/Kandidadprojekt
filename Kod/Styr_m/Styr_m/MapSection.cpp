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

Robot::Robot(int xPos, int yPos, Map* inMom, Communication* inComm) : MapSection(xPos, yPos, inMom){
	type = 'r';
	direction = 'f';
    
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
// Drives 

void Robot::drive(int speed){
    movementSpeed=speed;
	int output = floor(speed * 255 / 100);
	
	#if DEBUG == 0
		OCR2A = output;
		OCR2B = output;
	#endif	
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
	rotateActive = true;
	// Seft diffs to 0
	fwdDiff = 0;
	bwdDiff = 0;
	
    // Send request to sensor module to measure angle
    commObj->sendRotateRequest();
	
    // Turns
    changeGear('l');
	
	while (rotateActive)
	{
		drive(25);
	}
	
	// Stop rotation and set gear to forward
	drive(0);
	changeGear('f');
    
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
    changeGear('r');
	while (rotateActive)
	{
		drive(25);
	}
	
	// Stop rotation and set gear to forward
	drive(0);
	changeGear('f');
    
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
}

//-----------------------------------------

void Robot::turn(int pd){
    int output = floor(movementSpeed * 255 / 100);
	
#if DEBUG == 0
    OCR2A = output+pd; //Negative value on pd will turn left, positive right
    OCR2B = output-pd;
#endif
    
}


// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdValueIn(char* fwd){
	
    for (int i = 0; i < 3; i++) {
        pushBackChar(fwdSensor, fwd[i]);
    }
    
#if DEBUG == 1
    cout << "fwdValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << fwdSensor[it] << endl;
    }
#endif
}

void Robot::bwdValueIn(char* bwd){
	for (int i = 0; i < 3; i++) {
        pushBackChar(bwdSensor, bwd[i]);
    }
    
#if DEBUG == 1
    cout << "bwdValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << bwdSensor[it] << endl;
    }
#endif
}

void Robot::leftBackValueIn(char* left){
	for (int i = 0; i < 3; i++) {
        pushBackChar(leftBackSensor, left[i]);
    }
    
#if DEBUG == 1
    cout << "leftBackValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << leftBackSensor[it] << endl;
    }
#endif
}

void Robot::leftFrontValueIn(char* left){
	for (int i = 0; i < 3; i++) {
        pushBackChar(leftFrontSensor, left[i]);
    }
    
#if DEBUG == 1
    cout << "leftFrontValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << leftFrontSensor[it] << endl;
    }
#endif
}

void Robot::leftLongValueIn(char* left){
	for (int i = 0; i < 3; i++) {
        pushBackChar(leftLongSensor, left[i]);
    }
    
#if DEBUG == 1
    cout << "leftLongValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << leftLongSensor[it] << endl;
    }
#endif
}

void Robot::rightBackValueIn(char* right){
	for (int i = 0; i < 3; i++) {
        pushBackChar(rightBackSensor, right[i]);
    }
    
#if DEBUG == 1
    cout << "rightBackValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << rightBackSensor[it] << endl;
    }
#endif
}

void Robot::rightFrontValueIn(char* left){
	for (int i = 0; i < 3; i++) {
        pushBackChar(rightFrontSensor, left[i]);
    }
    
#if DEBUG == 1
    cout << "rightFrontValueIn" << endl;
    for (int it = 0; it < (int)strlen(fwdSensor); it++) {
        cout << rightFrontSensor[it] << endl;
    }
#endif
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
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
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
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
}

// -------------- To the left --------------------------

void Robot::setLeftClosed(){
	
	int output = meanValueArray(leftLongSensor,3)/40;
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
}

// -------------- To the right --------------------------

void Robot::setRightClosed(){
	
	int output = getRightDistance()/40;
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
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
    if (meanValueArray(fwdSensor,3) > 340 && meanValueArray(bwdSensor,3) > 340) {
        fwdReference = 300;
        bwdReference = 300;
    }
    if (fwdReference - meanValueArray(fwdSensor,3) >= 40){
        fwdReference=meanValueArray(fwdSensor,3);
        if (direction == 'f'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord, yCoord + 1);
            // Put robot in place
            mom->setSection(xCoord,yCoord + 1, this);
            // Update robot info about position
            yCoord++;
        }
        else if (direction == 'b'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord, yCoord - 1);
            // Put robot in place
            mom->setSection(xCoord,yCoord - 1, this);
            // Update robot info about position
            yCoord--;
        }
        else if (direction == 'r'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord + 1, yCoord);
            // Put robot in place
            mom->setSection(xCoord + 1,yCoord, this);
            // Update robot info about position
            xCoord++;
        }
        else if (direction == 'l'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord - 1, yCoord);
            // Put robot in place
            mom->setSection(xCoord - 1,yCoord, this);
            // Update robot info about position
            xCoord--;
        }
    }
    else if (meanValueArray(bwdSensor,3)-bwdReference <= 40){
        bwdReference=meanValueArray(bwdSensor,3);
        if (direction == 'f'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord, yCoord + 1);
            // Put robot in place
            mom->setSection(xCoord,yCoord + 1, this);
            // Update robot info about position
            yCoord++;
        }
        else if (direction == 'b'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord, yCoord - 1);
            // Put robot in place
            mom->setSection(xCoord,yCoord - 1, this);
            // Update robot info about position
            yCoord--;

        }
        else if (direction == 'r'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord + 1, yCoord);
            // Put robot in place
            mom->setSection(xCoord + 1,yCoord, this);
            // Update robot info about position
            xCoord++;
        }
        else if (direction == 'l'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord - 1, yCoord);
            // Put robot in place
            mom->setSection(xCoord - 1,yCoord, this);
            // Update robot info about position
            xCoord--;
        }

    }
}


// -----------------------------------------

void Robot::adjustPosition(){
    int pd;
    int error;
    int derivError;
    if (meanValueArray(rightFrontSensor,3)>80) { //right sensor out of range
        error=Ref-meanValueArray(leftFrontSensor,3);
        derivError=meanValueArray(leftFrontSensor,3)-previousLeftError;
        pd= Kp*error + Kd*derivError;
        previousLeftError=error; //Saves value for next differentiation
        if(getLeftDifference() < 0){
            turn(pd); //Turn right
        }
        else{
            turn(-pd); //Turn left
        }
    }
    else { //left Sensor out of range
        error=Ref-meanValueArray(rightFrontSensor,3);
        derivError=meanValueArray(rightFrontSensor,3)-previousRightError;
        previousRightError=error;
        pd= Kp*error + Kd*derivError;
        if(getRightDifference() < 0){
            turn(-pd); //Turn left
        }
        else{
            turn(pd); //Turn right
        }
    }
    
}

//----------------------------------
//Get difference between left and right side sensors

int Robot::getRightDifference(){
    int front;
    int back;
    front = meanValueArray(rightFrontSensor,3);
    back = meanValueArray(rightBackSensor,3);
    return front - back;

    
}

int Robot::getLeftDifference(){
    int front;
    int back;
    front = meanValueArray(leftFrontSensor,3);
    back = meanValueArray(leftBackSensor,3);
    return front - back;
    
    
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

int Robot::getRightDistance(){
    
    int output = meanValueArray(rightFrontSensor,3);
    output = output + meanValueArray(rightBackSensor,3);
    return output/2;
    
}








