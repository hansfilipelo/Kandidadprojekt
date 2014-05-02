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
	//command(0x0C); //turns of cursor
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

void Lcd::draw(unsigned char sign){
	   if(ready() && moveToggle){
		   //fulkod
			command(0x80);
            moveToggle = false;
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
		writeValue = (int)(writeBuffer[row][col]);
		col = col + 1;
		if (col==16){
			col = 0;
			row = row + 1;
		}
		if((col==16) & (row==4)){
			row=0;
			col=0;
		}
	}
	draw(writeValue);
}

void Lcd::insertSensorValuesToBuffer(unsigned char* inArray){
    //places sensorvalues in the correct position in the buffer
	int r = 0;
	int c = 3;
	
  	for(unsigned int i = 1; i<8; i++){
		switch(i)
		{
			case 0 : r = 0;
			break;
			case 1 : r = 2;
			break;
			case 2 : r = 0, c = 10;
			break;
			case 3 : r = 2;
			break;
			case 4 : r = 1;	
			break;
			case 5 : r = 3;
			break;
			case 6 : r = 2, c= 3;
			break;
		}
		writeBuffer[r][c] = inArray[i*3];
		writeBuffer[r][c+1] = inArray[i*3+1];
		writeBuffer[r][c+2] = inArray[i*3+2];
	}
}

/*		if(row == 0){
			writePosition = 0x80;
		}
		if(row == 1){
			writePosition = 0xc0;
		}
		if(row == 2) {
			writePosition = 0x90;
		}
		if(row == 3){
			writePosition = 0xd0;
		}
		if (col == 3) {
			writePosition = writePosition + 2;
		}
		else{
			writePosition = writePosition + 9;
		}

L1 array[3]
L2 array[6]
S1
S2
S3
S4
M1
*/


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
		return false;
	}
	else{
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

