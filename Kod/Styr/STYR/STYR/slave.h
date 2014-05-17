/*
 * slave.h
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 
#ifndef SLAVE
#define SLAVE

#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 0

#include <avr/io.h>
#include <avr/interrupt.h>

#endif

class Slave
{
	public:
	void SPI_Init();
	void SPI_Send();
	void SPI_Interupt();
	bool SPI_RecievedData();
	
	// Data arrives in order, datalength (data.length + codeword + type), codeword, type, data.
	unsigned char inDataArray[25]; 
	unsigned char outDataArray[25];
	unsigned int position = 0;
};

#endif