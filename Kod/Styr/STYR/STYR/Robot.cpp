//
//  Robot.cpp
//  
//
//  Created by Tobias Grundström on 2014-04-02.
//
//

#include "Robot.h"

Robot::Robot(int xPos, int yPos){
    xCoord=xPos;
    yCoord=yPos;
}

Robot:changeDirection(string direction){
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

Robot:drive(int speed){
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