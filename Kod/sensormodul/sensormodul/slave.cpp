/*
 * slave.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 

#include "slave.h"

void Slave::SPI_Init()
{
	DDRB = (1<<DDB6);	// Set MISO output, all others input
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE)|(1<<SPR0);
	SPCR &= ~(1<<MSTR);
	
	SPDR=0x00;		//reset SPDR
	
	
	EIMSK |= (1<<INT0);		//enable external interrupt
	EICRA |= (1<<ISC01)|(1<<ISC00);
	
	DDRB |= (1<<DDB2);	//set port B2 to output
	
	DDRD &= ~(1<<DDD7); //set port D7 to input
	
	//sets internal interrupt on pcint16
	PCMSK2 |= (1<<PCINT16);
	PCICR |= (1<<PCIE2);	
	DDRC |= (1<<DDC0);
	
	position = 0;		//reset position
	return;
}

void Slave::SPI_Send(){
	slaveSend = true;		//slave is sending
	position=0;				//reset position
	SPDR = outDataArray[0];	//sent first int in outDataArray
	PORTB |= (1<<PORTB2);	//interrupt to master
	PORTB &= ~(1<<PORTB2);	//clear interrupt to master
	return;
}