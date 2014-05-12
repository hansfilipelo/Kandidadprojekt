/*
 * slave.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 

#include "slave.h"

void Slave::SPI_Init()
{
    
#if DEBUG == 0
    
	/* Set MISO output, all others input */
	DDRB = (1<<DDB6);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE)|(1<<SPR0);
	SPCR &= ~(1<<MSTR);
	
	//dummy read to clear the register.
	SPDR=0x00;
	
	DDRB |= (1<<DDB2);
	
	//sets interupt on pcint16
	PCMSK2 |= (1<<PCINT16);
	PCICR |= (1<<PCIE2);	
	DDRC |= (1<<DDC0);
#endif
	
	position = 0;
	return;
}

void Slave::SPI_Send(){
	position=0;
	
#if DEBUG == 0
    SPDR = outDataArray[0];
    slaveSend= true;
	PORTB |= (1<<PORTB2);	//interrupt to master
	PORTB &= ~(1<<PORTB2);	//clear interrupt to master
#endif
    
	return;
}