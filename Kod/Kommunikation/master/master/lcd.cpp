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

void Lcd::draw(unsigned char location, unsigned char sign){
	if(ready() && moveToggle){
	command(location);
	moveToggle = 0;
	}
	if(ready() && (!moveToggle)){
	senddata(sign);
	moveToggle = 1;
	}
}

void Lcd::firstDraw{
    //DŒligt med delayer bšr endast gšas vid initiering.
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

void Lcd::updateS1(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0x83,m);
	draw(0x84,dm);
	draw(0x85,cm);
}

void Lcd::updateS2(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0xc3,m);
	draw(0xc4,dm);
	draw(0xc5,cm);
}

void Lcd::updateS3(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0x93,m);
	draw(0x94,dm);
	draw(0x95,cm);
}

void Lcd::updateS4(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0xd3,m);
	draw(0xd4,dm);
	draw(0xd5,cm);
}

void Lcd::updateL1(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0x8a,m);
	draw(0x8b,dm);
	draw(0x8c,cm);
}

void Lcd::updateL2(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0xca,m);
	draw(0xcb,dm);
	draw(0xcc,cm);
}

void Lcd::updateM1(char data1, char data2, char data3){
	int m = 0x30 + (int)(data1);
	int dm= 0x30 + (int)(data2);
	int cm= 0x30 + (int)(data3);
	
	draw(0x9a,m);
	draw(0x9b,dm);
	draw(0x9c,cm);
}

void Lcd::update(){
    int position;
    int row = 0;
    int value = 0x30 + (int)(writeBuffer[counter][row]);
    
    
    
    if(counter<16){
        position = 0x80;
    }
    else if(16<=counter<33){
        position = 0xc0;
        row=1;
    }
    else if(33<=counter<49) {
        position = 0x90;
        row=2;
    }
    else{
        position = 0xd0;
        row=3;
    }
    
    draw(position,value);
    counter++;
}

void Lcd::insertSensorValuesToBuffer(int sensor, char m, char dm, char cm){
    int row = 0;
    int col = 0;
    //places sensorvalues in the correct position in the buffer
    switch (sensor)
	{
		case 0 :
            col=10;
            row=1;
            break;
		case 1 :
            col=10;
            row=2;
            break;
		case 2 :
            col=3;
            row=1;
            break;
		case 3 :
            col=3;
            row=2;
            break;
		case 4 :
            col=3;
            row=3;
            break;
		case 5 :
            col=3;
            row=4;
            break;
		case 6 :
            col=10;
            row=3;
            break;
		default : //
            break;
	}
    writeBuffer[col][row] = m;
    writeBuffer[col+1][row] = dm
    writeBuffer[col+2][row] = cm;
}

bool Lcd::ready(){
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
}

