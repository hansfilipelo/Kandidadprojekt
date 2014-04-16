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

#include "masterMap.h"    
#include <string.h>
#include "bluetooth.h"
#include "spi.h"
#include "lcd.h"

bool toggle = false;
bool ReceiveFromSteer = false;
bool ReceiveFromSensor = false;

Map buffer;
Bluetooth Firefly;
Spi Bus(&Firefly,&buffer);
Lcd Display;

/*
*	Handeling data from modules
*/

void handleDataFromSteer(){
	ReceiveFromSteer=false;
	//memcpy(Firefly.outDataArray, Bus.inDataArray, 25);
	asm("");
	if(Firefly.outDataArray[1]=='M'){
		buffer.setColAsChar((char*)Bus.inDataArray);
		//int blowMe = buffer.setColAsChar();
		if((int)Firefly.outDataArray[2]==31){
			Firefly.sendMap();
		}
	}
}	

void handleDataFromSensor(){
	ReceiveFromSensor=false;
	int sensor = Bus.inDataArray[2];
	char m = Bus.inDataArray[3];
	char dm = Bus.inDataArray[4];
	char cm = Bus.inDataArray[5];
	
	switch (sensor)
	{
		case 0 : Display.updateL1(m,dm,cm);
		break;
		case 1 : Display.updateL2(m,dm,cm);
		break;
		case 2 : Display.updateS1(m,dm,cm);
		break;
		case 3 : Display.updateS2(m,dm,cm);
		break;
		case 4 : Display.updateS3(m,dm,cm);
		break;
		case 5 : Display.updateS4(m,dm,cm);
		break;
		case 6 : Display.updateM1(m,dm,cm);
		break;
		default : //
		break;
	}
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
    Firefly.setPointer(&Bus,&buffer);
	Display.drawSensorNames();
    
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