/*
 * master.cpp
 *
 * Created: 4/7/2014 9:13:57 AM
 *  Author: jened502
 */ 

// Precompiler flags
#ifndef __AVR_ATmega1284P__
#define DEBUG 1
#else
#define DEBUG 0
#endif

#if DEBUG == 0

#include <avr/io.h>
#include <avr/interrupt.h>

#endif

#include "Map.h"    
#include <string.h>
#include "bluetooth.h"
#include "spi.h"
#include "lcd.h"

bool toggle = false;
bool ReceiveFromSteer = false;
bool ReceiveFromSensor = false;

Bluetooth Firefly;
Spi Bus(&Firefly);

/*
*	Handeling data from modules
*/

void handleDataFromSteer(){
	ReceiveFromSteer=false;
	memcpy(Firefly.outDataArray, Bus.inDataArray, 25);
	asm("");
	if(Firefly.outDataArray[1]=='M'){
		Firefly.sendArray();
	}
}	

void handleDataFromSensor(){
	ReceiveFromSensor=false;	
}

#if DEBUG == 0
/*
*	INTERUPTS
*/
//bluetooth interupt
ISR(USART0_RX_vect)
{
	Firefly.receiveArray();
}

//Control module wants to send data
ISR(INT2_vect){
	cli();
	Bus.receiveArray(1);
	ReceiveFromSteer = true;
	sei();
}
//Sensor module wants to send data
ISR(INT1_vect){
	cli();
	Bus.receiveArray(0);
	ReceiveFromSensor = true;
	sei();
}
//Handle auto/manual button event
ISR(INT0_vect){
	cli();
	if(toggle){
		Bus.outDataArray[1]= 'q';
		toggle = false;
	}
	else{
		Bus.outDataArray[1]= 'a';
		toggle = true;
	}
	Bus.outDataArray[0]= 1;
	Bus.sendArray(1);
	sei();
}

#endif


int main(void)
{
    Firefly.setPointer(&Bus);
	Lcd Display;
	
	Display.drawSensorNames();
    Display.updateS1(1,9,1);
	sei();
    
	for(;;){
		asm("");
		Firefly.handle();
		if (ReceiveFromSteer){
			handleDataFromSteer();		
		}
		if(ReceiveFromSensor){
			handleDataFromSensor();
		}
	}
	
}