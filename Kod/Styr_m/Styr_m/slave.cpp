
#include "slave.h"


// Initiates SPI slave
void Slave::SPI_Init()
{
    
#if TESTING == 0
    
	/* Set MISO output, all others input */
	DDRB = (1<<DDB6);
	/* Enable SPI */
	SPCR = (1<<SPE)|(1<<SPIE)|(1<<SPR0);
	SPCR &= ~(1<<MSTR);
	
	//PORTB |= (1<<PORTB4); //Enable pull-up resistor
	
	//dummy l�sning f�r att rensa registret
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

// ----------------------------

// When slave wants to send - ask master through interrupt
void Slave::SPI_Send(){
	position=0;
	
#if TESTING == 0
    SPDR = outDataArray[0];
    slaveSend= true;
	PORTB |= (1<<PORTB2);	//interrupt to master
	PORTB &= ~(1<<PORTB2);	//clear interrupt to master
#endif
    
	return;
}

// ------------------------------------
// Interrupts needed in main for any slave

/*
ISR(SPI_STC_vect){
	sensormodul.position++;
	SPDR = sensormodul.outDataArray[sensormodul.position];
	sensormodul.inDataArray[sensormodul.position-1] = SPDR;
	
	if ((sensormodul.position == (sensormodul.inDataArray[0]+1))&(sensormodul.inDataArray[0]!= 0)){
		PORTC |= (1<<PORTC0);
	}
}

ISR(PCINT2_vect){
	handleInDataArray();
}

*/