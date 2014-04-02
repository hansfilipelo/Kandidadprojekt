//
//  Robot.cpp
//  
//
//  Created by Tobias Grundström on 2014-04-02.
//
//

#include <string.h>
#include "MapSection.h"

using namespace std;

// Construct ---------------------------
// Since this is a subclass - the MapSection constructor runs first.
// All we need to do is change type.

Robot::Robot(int xPos, int yPos, Map* inMom) : MapSection(xPos, yPos, inMom){
    type="robot";
}

// -------------------------------------
// Sets direction to travel

void Robot::changeDirection(string direction){
    if (direction == "backwards"){
		PORTD |= 0x10;
		PORTD &= ~0x20;
	}
	else if (direction == "right"){
		PORTD &= ~0x10;
		PORTD |= 0x20;
	}
	else if (direction == "left"){
		PORTD |= 0x10;
		PORTD |= 0x20;
	}
	else if (direction "forward"){
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