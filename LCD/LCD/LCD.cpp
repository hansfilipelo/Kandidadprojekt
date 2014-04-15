/*
 * LCD.cpp
 *
 * Created: 4/14/2014 11:13:42 AM
 *  Author: davha227
 */ 


#include <avr/io.h>

#define LCD_data PORTC

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

void LCD_init()
{
	LCD_data = 0x38; //Function set: 2 Line, 8-bit, 5x8 dots
	PORTA &= ~(0<<PORTA5)|(0<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(0<<PORTA7); //disable
	LCD_busy();
	
	LCD_data = 0x0F; //Display on, Curson blinking command
	PORTA &= ~(0<<PORTA5)|(0<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(0<<PORTA7); //disable
	LCD_busy();
	
	LCD_data = 0x01; //Clear LCD
	PORTA &= ~(0<<PORTA5)|(0<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(0<<PORTA7); //disable
	LCD_busy();
	
	LCD_data = 0x06; //Entry mode, auto increment with no shift
	PORTA &= ~(0<<PORTA5)|(0<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(0<<PORTA7); //disable, kanske inte måste ha denna
	LCD_busy();
}

void LCD_command(unsigned char var) // för att uföra olika kommandon
{
	LCD_data = var;
	PORTA &= ~(0<<PORTA5)|(0<<PORTA6);
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(0<<PORTA7); //disable
	LCD_busy(); //Wait for LCD to process the command
}

void LCD_senddata(unsigned char var)
{
	LCD_data = var; //Function set: 2 Line, 8-bit, 5x7 dots
	
	PORTA &= ~(0<<PORTA6);
	PORTA |= (1<<PORTA5); //enable
	PORTA |= (1<<PORTA7); //enable
	PORTA &= ~(0<<PORTA7); //disable
	LCD_busy(); //Wait for LCD to process the command
}

void LCD_sendstring(unsigned char *var)
{
	while(*var) //till string ends
	LCD_senddata(*var++); //send characters one by one
}

void LCD_builds(){
	LCD_command(0x48); //Load the location where we want to store
	LCD_senddata(0x04); //Load row 1 data
	LCD_senddata(0x0E); //Load row 2 data
	LCD_senddata(0x0E); //Load row 3 data
	LCD_senddata(0x0E); //Load row 4 data
	LCD_senddata(0x1F); //Load row 5 data
	LCD_senddata(0x00); //Load row 6 data
	LCD_senddata(0x04); //Load row 7 data
	LCD_senddata(0x00); //Load row 8 data
}

void LCD_build(unsigned char location, unsigned char *ptr){
	unsigned char i;
	if(location<8){
		LCD_command(0x40+(location*8));
		for(i=0;i<8;i++)
		LCD_senddata(ptr[ i ]);
	}
}

int main(void)
{
	DDRC = 0xFF; //port C till utgång
	DDRA = 0xC0; //port A pin 5-7 sätt till utgång
	PORTA |= (1<<PORTA7); // enable till 1
	
	LCD_init();
	//LCD_command(0x83); //bit7=1 resten ger cursorposition
	LCD_builds();
	
    while(1)
    {
        //TODO:: Please write your application code 
    }
}