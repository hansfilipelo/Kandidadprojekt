/*
 * Bluetooth.h
 *
 * Created: 4/7/2014 3:21:49 PM
 *  Author: niker917
 */ 
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
	unsigned char* getInDataArray();
	Spi(Bluetooth*,Map*);
	void init();
    char transfer(char, unsigned int);
	void sendArray(unsigned int);
    void receiveArray(unsigned int);
    
    // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data.
    unsigned char inDataArray[25];
    unsigned char outDataArray[25];
   
private:
    unsigned int  position = 0;
    Bluetooth* bluetoothPointer;
	Map* mapPointer;
};

#endif