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


#include <avr/io.h>
#include <util/delay.h>

char temp = 0x00;

#define LCD_data PORTC

void SetData(unsigned char var){
	temp = var & 0x01;
	if(temp == 0x01){
		PORTC |= (1<<PORTC0);
	}
	else{
		PORTC &= ~(1<<PORTC0);
	}
	temp = var >>1;
	temp = temp & 0x01;
	if(temp == 0x01){
		PORTC |= (1<<PORTC1);
	}
	else{
		PORTC &= ~(1<<PORTC1);
	}
	
	temp = var >>2;
	temp = temp & 0x01;
	
	if(temp == 0x01){
		PORTD |= (1<<PORTD4);
	}
	else{
		PORTD &= ~(1<<PORTD4);
	}
	
	temp = var >>3;
	temp = temp & 0x01;
	
	if(temp == 0x01){
		PORTD |= (1<<PORTD5);
	}
	else{
		PORTD &= ~(1<<PORTD5);
	}

	temp = var >>4;
	temp = temp & 0x01;
	if(temp == 0x01){
		PORTD |= (1<<PORTD6);
	}
	else{
		PORTD &= ~(1<<PORTD6);
	}
	
	temp = var >>5;
	temp = temp & 0x01;
	if(temp == 0x01){
		PORTD |= (1<<PORTD7);
	}
	else{
		PORTD &= ~(1<<PORTD7);
	}
	
	temp = var >>6;
	temp = temp & 0x01;
	
	if(temp == 0x01){
		PORTC |= (1<<PORTC6);
	}
	else{
		PORTC &= ~(1<<PORTC6);
	}

	temp = var >>7;
	temp = temp & 0x01;
	if(temp == 0x01){
		PORTC |= (1<<PORTC7);
	}
	else{
		PORTC &= ~(1<<PORTC7);
	}
}

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

void LCD_init(){
	SetData(0x38);
	//LCD_data = 0x38; //Function set: 2 Line, 8-bit, 5x8 dots
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	//LCD_busy();
	_delay_us(40);
	
	SetData(0x0F);
	//LCD_data = 0x0F; //Display on, Curson blinking command
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	//LCD_busy();
	_delay_us(40);
	
	SetData(0x01);
	//LCD_data = 0x01; //Clear LCD
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	//LCD_busy();
	_delay_ms(1.6);
	
	SetData(0x38);
	//LCD_data = 0x06; //Entry mode, auto increment with no shift
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	//LCD_busy();
	_delay_ms(40);
}

void LCD_command(unsigned char var) // för att uföra olika kommandon
{
	SetData(var);
	PORTA &= ~(1<<PORTA5)|(1<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	//LCD_busy(); //Wait for LCD to process the command
	_delay_us(40);
}

void LCD_senddata(unsigned char var)
{
	SetData(var); //Function set: 2 Line, 8-bit, 5x7 dots
	
	PORTA &= ~(1<<PORTA6);
	PORTA |= (1<<PORTA5); //Rs till 1
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(1<<PORTA7); //disable
	_delay_ms(40);
	
	//LCD_busy(); //Wait for LCD to process the command
}

void LCD_sendstring(unsigned char *var)
{
	while(*var) //till string ends
	LCD_senddata(*var++); //send characters one by one
}


void LCD_build(unsigned char location, unsigned char *ptr){
	unsigned char i;
	if(location<8){
		LCD_command(0x40+(location*8));
		for(i=0;i<8;i++)
		LCD_senddata(ptr[ i ]);
	}
}

void LCD_draw(unsigned char location, unsigned char sign){
	LCD_command(location);
	LCD_senddata(sign);
}

int main(void)
{	 
	DDRC = 0xC3; //portC pin 0,1,6,7 till utgångar
	DDRD = 0xF0;	//portD pin 4-7 som utgångar
	DDRA = 0xE0; //port A pin 5-7 sätt till utgång
	PORTA |= (1<<PORTA7); // enable till 1
	
	LCD_init();

	LCD_draw(0x8f,0x52);
	_delay_ms(4);
	LCD_draw(0x84,0x42);
    while(1);
}