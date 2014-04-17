/*
 * Bluetooth.h
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917
 */ 
#ifndef BLUETOOTH
#define BLUETOOTH

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>
#include "masterMap.h"

class Spi;

class Bluetooth
{
public:
	Bluetooth();
	void setPointer(Spi*,Map*);
	void init();
	void transmitByte(unsigned char);
	unsigned char receiveByte();
	void sendArray();
    void receiveArray();
    volatile void handle();
    volatile bool Btrec = false; 
	void sendMap();
    
    unsigned char pcHandle[25];
     // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data.
    unsigned char inDataArray[25] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    unsigned char outDataArray[25] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    
private:
    unsigned int position = 0;
	
	Spi* spiPointer;
	Map* mapPointer;
};

#endif