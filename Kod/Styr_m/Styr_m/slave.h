/*
 File: slave.h
 Purpose: Class implementing functions for communications with bus master.
 */

#ifndef SLAVE
#define SLAVE

#ifndef __AVR_ATmega1284P__
#define TESTING 1
#else
#define TESTING 0
#endif

#if TESTING == 0

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
	bool slaveSend = false;
	
	// Data arrives in order, datalength (data.length + codeword + type), codeword, type, data.
	unsigned char inDataArray[27]; 
	unsigned char outDataArray[27];
	unsigned int position;
};

#endif