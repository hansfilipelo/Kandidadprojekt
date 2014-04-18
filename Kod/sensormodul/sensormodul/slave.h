/*
 * slave.h
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917 davha227
 */ 
#ifndef SLAVE
#define SLAVE

#include <avr/io.h>
#include <avr/interrupt.h>

class Slave
{
	public:
	void SPI_Init();
	void SPI_Send();
	void SPI_Interupt();
	bool SPI_RecievedData();
	
	// Data arrives in order, datalength (data.length + codeword + type), codeword, type, data.
	unsigned char inDataArray[27]; 
	unsigned char outDataArray[27];
	unsigned int position;
};

#endif