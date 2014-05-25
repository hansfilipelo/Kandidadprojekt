/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: spi.cpp
*Purpose: Class for functions and buffers concerning bluetooth
*
*
********************************************************/
 
#include "spi.h"
#include "bluetooth.h"

/*
 *	Constructor. Takes pointers to the map and bluetooth classes.
 */

Spi::Spi(Bluetooth* ptr, Map* inMap){
	bluetoothPointer = ptr;
	mapPointer = inMap;
    init();
}


/*
 *	Initialization of SPI on module, configured as master.
 */

void Spi::init(){
    
    //Enable SPI, sets it low
	PRR0 &= ~(1<<PRSPI);
    
	//Sets slave select
	DDRA |= (1<<DDA0)|(1<<DDA1);
    
	/* Set MOSI and SCK output, all others input */
	DDRB |= (1<<DDB5)|(1<<DDB7)|(1<<DDB4);
	PORTA |= (1<<PORTA0);
	PORTA |= (1<<PORTA1);
	PORTB |= (1<<PORTB4);
    
	/* Enable SPI, Master, set clock rate fck/8 */
	SPCR |= (1<<SPE)|(1<<MSTR)|(1<<SPR0)|(1<<SPI2X);
    
	//Enable interupts
	EIMSK |= (1<<INT0)|(1<<INT2)|(1<<INT1);
	EICRA |= (1<<ISC01)|(1<<ISC00)|(1<<ISC11)|(1<<ISC10)|(1<<ISC21)|(1<<ISC20);
}


/*
 *	Transfer current SPDR register with slave's SPDR register.
 */


char Spi::transfer(char outData, unsigned int slave)
{
	char inData;
	//choose slave
	if (slave == 1){
		PORTA &= ~(1<<PORTA0);
	}
	else{
		PORTA &=~(1<<PORTA1);
	}
	/* Start transmission */
	SPDR = outData;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	//store possible indata
	inData = SPDR;
	//Synchronize the slave by pulling SS high
	if (slave == 1){
		PORTA |= (1<<PORTA0);
	}
	else{
		PORTA |= (1<<PORTA1);
	}
	return inData;
}


/*
 *	Perform the function transfer for the appropriate
 * 	amount of times specified as the length in the first
 * 	element of the outarray.
 */

void Spi::sendArray(unsigned int slave){
	
	unsigned int length = 0;
	length = outDataArray[0];
	for (unsigned int i=0; i<=length; i++)
	{
		
		transfer(outDataArray[i], slave);
	}
}


/*
 *	Perform the transfer function as many time as needed specified
 *	by the first incoming byte.
 */

void Spi::receiveArray(unsigned int slave){
	
	inDataArray[0] = transfer(0x00,slave);
	unsigned int length = inDataArray[0];
	_delay_us(6); // delay to ensure enough time between transmissions.
	for (unsigned int i=1; i<=length; i++)
	{
		inDataArray[i] = transfer(0x00, slave);
		_delay_us(6);
	}
}

