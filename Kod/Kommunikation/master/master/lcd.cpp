/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: lcd.cpp
*Purpose: 
*
*Created by: Niklas Ericson (niker917), David Habrman (davha227)
*	Jens Edhammer (jened502)
*
********************************************************/

/*
 *  Cables and ports.
 *  Rs -> PORTA5
 *  R/W -> PORTA6
 *  E -> PORTA7
 *  DB0 -> PORTC0
 *  DB1 -> PORTC1
 *  DB2 -> PORTD4
 *  DB3 -> PORTD5
 *  DB4 -> PORTD6
 *  DB5 -> PORTD7
 *  DB6 -> PORTC6
 *  DB7 -> PORTC7
 */

#include "lcd.h"

Lcd::Lcd(){
	init();
	command(0x0C); //turns off cursor
	drawSensorNames();
}

void Lcd::SetData(unsigned char var){
	shift = var & 0x01;
	if(shift == 0x01){
		PORTC |= (1<<PORTC0);
	}
	else{
		PORTC &= ~(1<<PORTC0);
	}
	shift = var >>1;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTC |= (1<<PORTC1);
	}
	else{
		PORTC &= ~(1<<PORTC1);
	}
	
	shift = var >>2;
	shift = shift & 0x01;
	
	if(shift == 0x01){
		PORTD |= (1<<PORTD4);
	}
	else{
		PORTD &= ~(1<<PORTD4);
	}
	
	shift = var >>3;
	shift = shift & 0x01;
	
	if(shift == 0x01){
		PORTD |= (1<<PORTD5);
	}
	else{
		PORTD &= ~(1<<PORTD5);
	}
    
	shift = var >>4;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTD |= (1<<PORTD6);
	}
	else{
		PORTD &= ~(1<<PORTD6);
	}
	
	shift = var >>5;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTD |= (1<<PORTD7);
	}
	else{
		PORTD &= ~(1<<PORTD7);
	}
	
	shift = var >>6;
	shift = shift & 0x01;
	
	if(shift == 0x01){
		PORTC |= (1<<PORTC6);
	}
	else{
		PORTC &= ~(1<<PORTC6);
	}
    
	shift = var >>7;
	shift = shift & 0x01;
	if(shift == 0x01){
		PORTC |= (1<<PORTC7);
	}
	else{
		PORTC &= ~(1<<PORTC7);
	}
}

void Lcd::init(){
    DDRC |= 0xC3; //portC pin 0,1,6,7 till utgångar
	DDRD |= 0xF0;	//portD pin 4-7 som utgångar
	DDRA |= 0xE0; //port A pin 5-7 sätt till utgång
	PORTA |= (1<<PORTA7); // enable till 1
    
	SetData(0x38);
    //Function set: 2 Line, 8-bit, 5x8 dots
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
	
	SetData(0x0F);
	//Display on, Curson blinking command
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
	
	SetData(0x01);
	//Clear LCD
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
	
	SetData(0x06);
	//Entry mode, auto increment with no shift
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	_delay_ms(100);
}

void Lcd::command(unsigned char var){
	
	DDRC |=(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7);
	DDRD |=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	SetData(var);
	
	PORTA &= ~(1<<PORTA6);
	PORTA &= ~(1<<PORTA5);
	_delay_us(1);
	PORTA |=(1<<PORTA7);
	_delay_us(1);
	PORTA &= ~(1<<PORTA7);
	_delay_us(1);
	PORTA |= (1<<PORTA7);
}

void Lcd::senddata(unsigned char var){
	SetData(var); //Function set: 2 Line, 8-bit, 5x7 dots
	
	PORTA &= ~(1<<PORTA6);
	PORTA |= (1<<PORTA5); //Rs till 1
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
}

void Lcd::draw(unsigned char location, unsigned char sign){
	
        if(ready() && moveToggle){
            command(location);
            moveToggle = 0;
			return;
		}
		if(ready() && (!moveToggle)){
            asm("");
			senddata(sign);
            moveToggle = 1;
			drawSucceded = true;
		}
}

void Lcd::firstDraw(unsigned char location, unsigned char sign){
    _delay_us(40);
    command(location);
    _delay_us(40);
    senddata(sign);
}

void Lcd::drawSensorNames(){
	
    //rad 1
	firstDraw(0x80,'F');
	firstDraw(0x81,'l');
	firstDraw(0x87,'R');
	firstDraw(0x88,'f');
	
	//rad 2
	firstDraw(0xc0,'F');
	firstDraw(0xc1,'s');	
	firstDraw(0xc7,'R');
	firstDraw(0xc8,'b');

	//rad 3
	firstDraw(0x97,'M');
	firstDraw(0x98,'l');
	firstDraw(0x90,'B');
	firstDraw(0x91,'l');
    
	//rad 4
	//prints S4
	firstDraw(0xd7,'I');
	firstDraw(0xd8,'D');
	firstDraw(0xd0,'B');
	firstDraw(0xd1,'s');
}

unsigned int Lcd::getNewPosition(int col, int row){
	unsigned int position;
	
	switch(row){
		case 0 :  position = 0x80;
			break;
		case 1 :  position = 0xc0;
			break;
		case 2 :  position = 0x90;
			break;
		case 3 :  position = 0xd0;
			break;
	} 
	if (col==3){
		position = position + 3;
	}
	else{
		position = position + 10;
	}
	return position;
}

void Lcd::update(){
	char nextChar;
	if (drawSucceded){
		drawSucceded=false;
		if (internalCounter == 0){
			row = getRow(sensorCounter);
			col = getCol(sensorCounter);
			position = getNewPosition(col,row);
		}
		else if ((internalCounter == 1) || (internalCounter == 2)){
			position = position + 1;
			col = col + 1;
		}
		else{
			internalCounter = 0;
			if(sensorCounter==8){
				sensorCounter = 0;
				drawSucceded = true;
				bufferWritten = true;
				return;
			}
			else{
			sensorCounter = sensorCounter + 1;
			drawSucceded=true;
			return;
			}
		}
		internalCounter++;
		nextChar = writeBuffer[row][col];
	}
	draw(position,nextChar);	
}

void Lcd::insertSensorValuesToBuffer(unsigned char* inArray){
	//places sensorvalues in the correct position in the buffer
	int r = 0;
	int c = 0;
	
	for(unsigned int i = 1; i<9; i++){
		switch(i)
		{
			case 1 :	r = 0;
						c = 3;	
						break;
			case 2 :	r = 2;
						c = 3;
						break;
			case 3 :	r = 3;
						c = 3;
						break;
			case 4 :	r = 1;
						c = 10;
						break;
			case 5 :	r = 1;
						c = 3;
						break;
			case 6 :	r = 0;
						c = 10;
						break;
			case 7 :	r = 2;
						c = 10;
						break;
			case 8 :	r = 3;
						c = 10;
						break;
		}				
		writeBuffer[r][c] = 0x30 + (int)inArray[i*3];
		writeBuffer[r][c+1] = 0x30 + (int)inArray[i*3+1];
		writeBuffer[r][c+2] = 0x30 + (int)inArray[i*3+2];
	}
}

int Lcd::getCol(int sensor){
    int col = 0;
    if((sensor <= 2) || (sensor == 4)){
        col = 3;
    }
    else{
        col = 10;
    }
    return col;
}

int Lcd::getRow(int sensor){
    int row;
    if((sensor == 0) || (sensor == 5)){
        row = 0;
    }
    else if((sensor == 3) || (sensor == 4)){
        row = 1;
    }
    else if((sensor == 1) || (sensor == 6)){
        row = 2;
    }
    else{
        row = 3;
    }
    return row;
}

bool Lcd::ready(){

	PORTA &=~(1<<PORTA5);
	PORTA |=(1<<PORTA6);
	_delay_us(1);
	//Make it possible to read from DB0-DB7
	DDRC &=~((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7));
	DDRD &=~((1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7));

	PORTA |=(1<<PORTA7);
	_delay_us(1);
	//Data should now be ready to read from PORTC7.
	
	volatile bool temp = PINC & 0x80;
	
	
	DDRC |=(1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7);
	DDRD |=(1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7);
	
	if(temp){
		PORTA &=~(1<<PORTA7);
		return false;
	}
	else{
		PORTA &=~(1<<PORTA7);
		return true;
	}
	
}