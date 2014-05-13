/*
 * bluetooth.cpp
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 

#include "spi.h"
#include "bluetooth.h"

Spi::Spi(Bluetooth* ptr, Map* inMap){
	bluetoothPointer = ptr;
	mapPointer = inMap;
    init();
}

void Spi::init(){
    
    //Enable SPI, set it low
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
 *  Exchange char with selected slave. SlaveSelect is pulled low.
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
 *  Send the complete array stored in outDataArray
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
 *  ReceiveArray from slave by sending them zeros and saving data we recieve back.
 *  The delay is necessary to ensure no data-"enveloping" and that both units 
 *  has processed and readied the necessary data. Strange behaviour on
 *  transmissions? Try increasing the delays before anything else.
 */


void Spi::receiveArray(unsigned int slave){
	
	inDataArray[0] = transfer(0x00,slave);
	unsigned int length = inDataArray[0];
	_delay_us(4);
	for (unsigned int i=1; i<=length; i++)
	{
		inDataArray[i] = transfer(0x00, slave);
		_delay_us(4);
	}
}

/*
 *  This function requests a row from the Steermodule. Due to change this function
 *  is no longer viable or supported. (remove?)
 */


void Spi::requestRow( unsigned int row)
{
	outDataArray[0] = 2;
	outDataArray[1] = 'm';
	outDataArray[2] =  row;
	
	sendArray(1);	
}

