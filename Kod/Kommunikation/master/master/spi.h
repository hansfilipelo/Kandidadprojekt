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

class Spi
{
    
public:
	void init();
    void transfer();
	void sendArray();
    void receiveArray();
    
    
    // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data.
    unsigned char inDataArray[25];
    unsigned char outDataArray[25];
   
private:
    unsigned int  position = 0;
    
    
};

#endif