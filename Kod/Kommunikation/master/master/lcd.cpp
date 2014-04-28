/*
 * lcd.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 *
 *  Virat kablage
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
	command(0x0C); //turns of cursor
	
    for (int it = 0; it < 4; it++) {
	    for (int i = 0; i < 16; i++) {
		   writeBuffer[it][i] = 2;
	    }
    }
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
// för att uföra olika kommandon
void Lcd::command(unsigned char var){
	SetData(var);
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	//LCD_busy(); //Wait for LCD to process the command
}

void Lcd::senddata(unsigned char var){
	SetData(var); //Function set: 2 Line, 8-bit, 5x7 dots
	
	PORTA &= ~(1<<PORTA6);
	PORTA |= (1<<PORTA5); //Rs till 1
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	//LCD_busy(); //Wait for LCD to process the command
}

void Lcd::firstDraw(unsigned char location, unsigned char sign){
    //DŒligt med delayer bšr endast gšras vid initiering.
    _delay_us(40);
    command(location);
    _delay_us(40);
    senddata(sign);
}

/*
 *Skriver ut namnen på sensorerna
 *	Bit 1
 *	8->rad1
 *	c->rad2
 *	9->rad3
 *	d->rad4
 *
 *	Bit 2 bestämmer vart på raden (0->f)
 */
void Lcd::drawSensorNames(){
	
    //rad 1
	//prints S1
	firstDraw(0x80,0x53);
	firstDraw(0x81,0x31);
	//prints L1
	firstDraw(0x87,0x4c);
	firstDraw(0x88,0x31);
	//prints KP
	firstDraw(0x8d,0x4b);
	firstDraw(0x8e,0x50);
	
	//rad 2
	//prints S2
	firstDraw(0xc0,0x53);
	firstDraw(0xc1,0x32);
	//prints L2
	firstDraw(0xc7,0x4c);
	firstDraw(0xc8,0x32);
	//prints KD
	firstDraw(0xcd,0x4b);
	firstDraw(0xce,0x44);
	
	//rad 3
	//prints S3
	firstDraw(0x90,0x53);
	firstDraw(0x91,0x33);
	//prints M1
	firstDraw(0x97,0x4d);
	firstDraw(0x98,0x31);
    
	//rad 4
	//prints S4
	firstDraw(0xd0,0x53);
	firstDraw(0xd1,0x34);
}

void Lcd::draw(unsigned char location, unsigned char sign){
	
        if(ready() && moveToggle){
            command(location);
            moveToggle = 0;
        }
        if(ready() && (!moveToggle)){
            senddata(sign);
            moveToggle = 1;
			drawSucceded = true;
    }
}

void Lcd::update(){
	if(drawSucceded){
		drawSucceded = false;
		int row = getRow(sensorCounter);
		int col = getCol(sensorCounter);
    
		if(row == 0){
			writePosition = 0x80;
		}
		if(row == 1){
			writePosition = 0xc0;
		}
		if(row == 2) {
			writePosition = 0x90;
		}
		else{
			writePosition = 0xd0;
		}
    
		if (col==3) {
			writePosition = writePosition + 2;
		}
		else{
			writePosition = writePosition + 9;
		}
		internalCounter = internalCounter + 1;
		writeValue = (int)(writeBuffer[row][col]);
		if(internalCounter <= 3){
			writePosition = writePosition + 1;
			col = col + 1;
		}
		else{
			internalCounter = 0;
			sensorCounter = sensorCounter + 1;
		}
	}
	draw(writePosition,writeValue);
}

void Lcd::insertSensorValuesToBuffer(unsigned char* inArray){
    //places sensorvalues in the correct position in the buffer
	for(unsigned int i = 0; i<7; i++){
		    int row = getRow(i+1);
		    int col = getCol(i+1);
		    
		    writeBuffer[row][col] = inArray[i+2];
		    writeBuffer[row][col+1] = inArray[i+3];
		    writeBuffer[row][col+2] = inArray[i+4];
	}
}

int Lcd::getCol(int sensor){
    int col = 0;
    if((sensor <= 1) or (sensor == 6)){
        col = 10;
    }
    else{
        col = 3;
    }
    return col;
}

int Lcd::getRow(int sensor){
    int row;
    if((sensor == 0) or (sensor == 2)){
        row = 1;
    }
    else if((sensor == 1) or (sensor == 3)){
        row = 2;
    }
    else if((sensor == 4) or (sensor == 6)){
        row = 3;
    }
    else{
        row = 4;
    }
    return row;
}


bool Lcd::ready(){

/*
 * lcd.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 *
 *  Virat kablage
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

	//Make it possible to read from DB0-DB7
	DDRC &=~((1<<PORTC0)|(1<<PORTC1)|(1<<PORTC6)|(1<<PORTC7));
	DDRD &=~((1<<PORTD4)|(1<<PORTD5)|(1<<PORTD6)|(1<<PORTD7));
	PORTA &=~(1<<PORTA5);
	PORTA |=(1<<PORTA6);
	_delay_us(1);
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

	
	
	/*
    PORTA |=(1<<PORTA7);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    PORTA &=~(1<<PORTA7);
    asm("nop");
    asm("nop");
    asm("nop");
    asm("nop");
    PORTA |=(1<<PORTA7);
    PORTA |= (1<<PORTA6); //enable
    PORTA &= ~(1<<PORTA5);
    bool temp = PINC & 0x80;
    if(!temp){
        PORTA &= ~(1<<PORTA6);
        return true;
    }
    else{
        PORTA &= ~(1<<PORTA6);
        return false;
    }
	*/

