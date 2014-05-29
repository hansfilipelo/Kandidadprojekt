/******************************************************
*
*Code was produced as part of the project MapMaster2001
*
*File: bluetooth.cpp
*Purpose: Class for functions and buffers concerning bluetooth
*
*
********************************************************/


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
	volatile bool getMap = false; 
	volatile int mapNumber = 0;
	bool autonom = false;
    
    volatile bool mapDone = false;
	unsigned int rowToSend = 0;
	volatile bool rdyForRow = true;
    
    unsigned char pcHandle[27];
     // Data arrives in order, datalength (data.length + codeword + arg), codeword, arg, data.
    unsigned char inDataArray[27] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    unsigned char outDataArray[27] = {'0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0'};
    
private:
    unsigned int position = 0;
	
	Spi* spiPointer;
	Map* mapPointer;
};

#endif