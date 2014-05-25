/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: spi.h
*Purpose: Class for functions and buffers concerning bluetooth
*
*Created by: Niklas Ericson (niker917), David Habrman (davha227)
*	Jens Edhammer (jened502)
*
********************************************************/
 
#ifndef SPI
#define SPI

#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 14.7456E6
#include <util/delay.h>
#include "masterMap.h"
#include "../../../Styr_m/Styr_m/Abstraction.h"

class Bluetooth;

class Spi
{
    
public:
	Spi(Bluetooth*,Map*);
	void init();
    char transfer(char, unsigned int);
	void sendArray(unsigned int);
    void receiveArray(unsigned int);
    
    // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data.
    unsigned char inDataArray[27];
    unsigned char outDataArray[27];
	unsigned char buffer[27];
	volatile unsigned int latestRow = 31;
    volatile bool waitingForW = false;
	volatile bool gyroActive = false;
	volatile bool rfidActive = false;
   
private:
    unsigned int  position = 0;
    Bluetooth* bluetoothPointer;
	Map* mapPointer;
};

#endif