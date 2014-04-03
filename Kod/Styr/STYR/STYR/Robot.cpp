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

using namespace std;

// Construct ---------------------------
// Since this is a subclass - the MapSection constructor runs first.
// All we need to do is change type.

Robot::Robot(int xPos, int yPos, Map* inMom) : MapSection(xPos, yPos, inMom){
    type='r';
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
    if (speed == 0){
		speed = 25;
	}
	else{
		speed = 0;
	}
	int output = floor(speed * 255 / 100);
	
	OCR2A = output;
	OCR2B = output;
}

// ------------------------------------
// Gets sensorvalues and activates SLAM functions

void Robot::sensorValuesIn(double fwd, double bwd, double left, double leftLong, double right, double phiDot)
{
	
}







