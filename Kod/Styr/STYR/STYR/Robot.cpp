//
//  Robot.cpp
//  
//
//  Created by Tobias Grundström on 2014-04-02.
//
//
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>
#include "MapSection.h"
#include "MapSection.cc"

using namespace std;

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
		PORTD |= 0x10;
		PORTD &= ~0x20;
	}
	else if (direction == 'r'){
		PORTD &= ~0x10;
		PORTD |= 0x20;
	}
	else if (direction == 'l'){
		PORTD |= 0x10;
		PORTD |= 0x20;
	}
	else if (direction == 'f'){
		PORTD &= ~0x10;
		PORTD &= ~0x20;
	}
    
}

// ------------------------------------
// Drives

void Robot::drive(int speed){
	int output = floor(speed * 255 / 100);
	
	OCR2A = output;
	OCR2B = output;
}

// ------------------------------------
// Gets sensorvalues and will probably later activate SLAM functions

void Robot::fwdValueIn(int fwd){
	fwdSensor = pushBackInt(fwdSensor, fwd);
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

// ------------------------------------
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


