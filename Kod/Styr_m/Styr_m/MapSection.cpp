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

void Robot::drive(){
	if (speed < 101)
	{
		int output = floor(speed * 255 / 100);
		
		#if DEBUG == 0
		OCR2A = output;
		OCR2B = output;
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
		drive();
	}
	
	// Stop rotation and set gear to forward
	setSpeed(0);
	drive();
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
	commObj->sendRotateRequest();
	
    // Turns
    changeGear('r');
	while (rotateActive)
	{
		drive();
	}
	
	// Stop rotation and set gear to forward
	setSpeed(0);
	drive();
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
	
	int pdOut = pd * movementSpeed * 0.01;
	
	#if DEBUG == 0
	OCR2A = output+pdOut; //Negative value on pd will turn left, positive right
	OCR2B = output-pdOut;
	#endif
}


// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdValueIn(char fwd[3]){
    fwdSensor = 100 * fwd[0];
    fwdSensor += 10 * fwd[1];
    fwdSensor += fwd[2];

#if DEBUG == 1
    cout << "fwdValueIn" << endl;
    cout << fwdSensor << endl;
#endif
}

void Robot::bwdValueIn(char* bwd){
    bwdSensor = 100 * bwd[0];
    bwdSensor += 10 * bwd[1];
    bwdSensor += bwd[2];
    
#if DEBUG == 1
    cout << "bwdValueIn" << endl;
    cout << bwdSensor << endl;
#endif
}

void Robot::leftBackValueIn(char left[3]){
    leftBackSensor = 100 * left[0];
    leftBackSensor += 10 * left[1];
    leftBackSensor += left[2];
    
#if DEBUG == 1
    cout << "leftBackValueIn" << endl;
    cout << leftBackSensor << endl;
#endif
}

void Robot::leftFrontValueIn(char left[3]){
    leftFrontSensor = 100 * left[0];
    leftFrontSensor += 10 * left[1];
    leftFrontSensor += left[2];
    
#if DEBUG == 1
    cout << "leftFrontValueIn" << endl;
    cout << leftFrontSensor << endl;
#endif
}

void Robot::leftLongValueIn(char left[3]){
    leftLongSensor = 100 * left[0];
    leftLongSensor += 10 * left[1];
    leftLongSensor += left[2];
    
#if DEBUG == 1
    cout << "leftLongValueIn" << endl;
    cout << leftLongSensor << endl;
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
    
    if ( fwdSensor > 340 && bwdSensor > 340 ) {
        output = 300/40;
    }
    else{
        output = fwdSensor/40;
    }
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		//mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		//mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		//mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		//mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}	
}

// ------------------------------------------------

void Robot::setBwdClosed(){
	
	int output = 0;
	
	if ( fwdSensor > 340 && bwdSensor > 340 ) {
		output = 300/40;
	}
	else{
		output = bwdSensor/40;
	}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		//mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		//mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		//mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		//mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
}

// -------------- To the left --------------------------

void Robot::setLeftClosed(){
	
		int output = 0;
		
		if ( fwdSensor > 340 && bwdSensor > 340 ) {
			output = 300/40;
		}
		else{
			output = leftLongSensor/40;
		}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		//mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		//mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		//mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord - i - 1, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		//mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
}

// -------------- To the right --------------------------

void Robot::setRightClosed(){
	
	int output = 0;
	
	if ( fwdSensor > 340 && bwdSensor > 340 ) {
		output = 300/40;
	}
	else{
		output = getRightDistance()/40;
	}
	
	// Set closed section output + 1 steps away from robot.
	// Direction 0->y->17, "fwd"
	if (direction == 'f'){
		//mom->convertSection(xCoord - output - 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord - i - 1,yCoord, 'e');
        }
	}
	// Direction 17->y->0, "bwd"
	else if (direction == 'b'){
		//mom->convertSection(xCoord + output + 1,yCoord, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord + i + 1,yCoord, 'e');
        }
	}
	// Direction 0->x->32, "right"
	else if (direction == 'r'){
		//mom->convertSection(xCoord,yCoord + output + 1, 'c');
        
        // Set every section between robot and wall as empty
        for (int i = 0; i < output; i++) {
            mom->convertSection(xCoord,yCoord + i + 1, 'e');
        }
	}
	// Direction 32->x->0, "left"
	else if (direction == 'l'){
		//mom->convertSection(xCoord,yCoord - output - 1, 'c');
        
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
	
	
	
    if (fwdReference - fwdSensor >= 40){
        fwdReference=fwdSensor;
        if (direction == 'f'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord, yCoord + 1);
            // Put robot in place
            mom->setSection(xCoord,yCoord + 1, this);
            // Update robot info about position
            yCoord++;
			// Set a new reference value
			this->setFwdReference();
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
			// Set a new reference value
			this->setFwdReference();
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
			// Set a new reference value
			this->setFwdReference();
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
			// Set a new reference value
			this->setFwdReference();
        }
    }
	
    else if (bwdSensor-bwdReference <= 40){
        bwdReference=bwdSensor;
        if (direction == 'f'){
            // Place back the section we stand in
            mom->setSection(previousSection->getX(), previousSection->getY(), previousSection);
            // Get a new prev section
            previousSection = mom->getPos(xCoord, yCoord + 1);
            // Put robot in place
            mom->setSection(xCoord,yCoord + 1, this);
            // Update robot info about position
            yCoord++;
			// Set a new reference value
			this->setBwdReference();
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
			// Set a new reference value
			this->setBwdReference();
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
			// Set a new reference value
			this->setBwdReference();
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
			// Set a new reference value
			this->setBwdReference();
        }

    }
}


// -----------------------------------------

void Robot::adjustPosition(){
	volatile int pd = 0;
	volatile int frontError = 0;
	volatile int derivError = 0;
	volatile int backError = 0;
	volatile int error = 0;
	volatile int both = 0;
	
    if (rightFrontSensor > 80) { //right sensor out of range
        frontError=Ref-leftFrontSensor;
		backError=Ref-leftBackSensor;
		
		both = frontError + backError;
		error = both/2;
		
        derivError=frontError - backError;
		pd= Kp*error + Kd*derivError;
        //previousLeftError=error; //Saves value for next differentiation
        
        turn(-pd);
        
        /*if(getLeftDifference() < 0){
            turn(-pd); //Turn right
        }
        else{
            turn(pd); //Turn left
        }*/
    }
    else { //right Sensor in range
        frontError=Ref-rightFrontSensor;
		backError=Ref-rightBackSensor;
		
        derivError = frontError - backError;
        
		// previousRightError=error;
		
		both = frontError + backError;
		error = both/2;
		
        pd= Kp*error + Kd*derivError;
		robotTempPd = pd;
        
        // Turn
        turn(pd);
        /*if(getRightDifference() < 0){
            turn(pd); //Turn left
        }
        else{
            turn(-pd); //Turn right
        }*/
    }
    
}

//----------------------------------
//Get difference between left and right side sensors

int Robot::getRightDifference(){
    int front;
    int back;
    front = rightFrontSensor;
    back = rightBackSensor;
    return front - back;

    
}

int Robot::getLeftDifference(){
    int front;
    int back;
    front = leftFrontSensor;
    back = leftBackSensor;
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
    
    int output = rightFrontSensor;
    output = output + rightBackSensor;
    return output/2;
    
}

void Robot::setControlParameters(double inputKp, double inputKd, int inputRef){
    Kp=inputKp;
    Kd=inputKd;
	
    Ref=inputRef;
}

void Robot::setSpeed(int inSpeed)
{
	speed = inSpeed;
	movementSpeed=inSpeed;
}

// ------------------------ 
// Sets reference for mapping

void Robot::setFwdReference(){
	if (fwdSensor > 340)
	{
		fwdReference = 300;
	}
	else {
		fwdReference = fwdSensor;
	}
}

void Robot::setBwdReference(){
		if (bwdSensor > 340)
	{
		bwdReference = 300;
	}
	else {
		bwdReference = bwdSensor;
	}
}

void Robot::setUserSpeed(int inSpeed)
{
	speed = inSpeed;
	userSpeed = inSpeed;
	movementSpeed = inSpeed;
}

// --------------------------

bool Robot::isWallRight(){
    
    if ( rightFrontSensor > 30 || rightBackSensor > 30 ){
        return false;
    }
    else {
        return true;
    }
}

// --------------------------

bool Robot::isCornerRight(){
    
    if ( rightFrontSensor > 40 && rightBackSensor < 25 ){
        return true;
    }
    else {
        return false;
    }
}

// --------------------------

bool Robot::isWallFwd(){
    
    if ( fwdSensor < 40 ){
        return true;
    }
    else{
        return false;
    }
    
}

// --------------------------

bool Robot::isWallLeft(){
    
    if ( leftFrontSensor > 30 || leftFrontSensor > 30 ){
        return false;
    }
    else {
        return true;
    }
}

//













