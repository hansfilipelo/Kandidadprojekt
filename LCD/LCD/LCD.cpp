/*
 * LCD.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 
  Rs -> PORTA5
  R/W -> PORTA6
  E -> PORTA7
  DB0 -> PORTC0
  DB1 -> PORTC1
  DB2 -> PORTD4
  DB3 -> PORTD5
  DB4 -> PORTD6
  DB5 -> PORTD7
  DB6 -> PORTC6
  DB7 -> PORTC7
 */ 
#include "LCD.h"

Lcd::Lcd(){
	DDRC |= 0xC3;			//portC pin 0,1,6,7 till utgångar
	DDRD |= 0xF0;			//portD pin 4-7 som utgångar
	DDRA |= 0xE0;			//port A pin 5-7 sätt till utgång
	PORTA |= (1<<PORTA7);	//enable till 1
	LCD_init();
	
	
	LCD_draw(0x80,0x53);	//8->rad1, 0-f väljer vart på raden
	LCD_draw(0x9f,0x53);	//9->rad3
	LCD_draw(0xc0,0x53);	//c->rad2
	LCD_draw(0xdf,0x53);	//d->rad4
}

void Lcd::SetData(unsigned char var){	//plockar ut varje bit för sig i ett hextal och skickar ut det på rätt portar till LCD:n
	shift = var & 0x01;					//skiftar och and:ar med 0x01 för att få ut önskad bit
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

//den här funktionen borde kollas på, är snyggare att kolla flagga än att lägg in delays
/*
void LCD_busy(){
	PORTC = 0x80;
	PORTA |= (1<<PORTA7); //enable
	PORTA |= (1<<PORTA6);
	PORTA &= ~(0<<PORTA5);

	while(PORTC7){ //read busy flag again and again till it becomes 0
		PORTA &= ~(0<<PORTA7); //disable
		PORTA |= (1<<PORTA7); //enable
	}
}
*/

void Lcd::LCD_init(){
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

void Lcd::LCD_command(unsigned char var)	// för att uföra olika kommandon, oftast önskad plats
{
	SetData(var);
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
}

void Lcd::LCD_senddata(unsigned char var)	// för att skicka tecken till LCD
{
	SetData(var);
	
	PORTA &= ~(1<<PORTA6);
	PORTA |= (1<<PORTA5); //Rs till 1
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(100);
}


void Lcd::LCD_draw(unsigned char location, unsigned char sign){	//för att skriva på LCD
	LCD_command(location);					//sätter platsen på LCD
	LCD_senddata(sign);						//skickar önskat tecken till LCD
}


int main(){
		Lcd hej;
		for(;;);
	};