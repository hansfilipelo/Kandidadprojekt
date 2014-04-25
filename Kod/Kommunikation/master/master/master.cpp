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
	memcpy(Firefly.outDataArray, Bus.inDataArray,27);
	
	
	
	if(Firefly.outDataArray[1]=='M'){
		
		memcpy(buffer.mapArea[Firefly.outDataArray[2]],Firefly.outDataArray,27);
		
		if((int)Firefly.mapNumber==31){
			Firefly.sendMap();
			Firefly.getMap =false;
			Firefly.mapNumber = 0;
		}else{
			Firefly.mapNumber++;
			Firefly.getMap =true;
		}
		
	}
}	

void handleDataFromSensor(){
	ReceiveFromSensor=false;
	memcpy(Firefly.outDataArray, Bus.inDataArray,27);
	
	if(Firefly.outDataArray[1] == 'S'){
	Firefly.sendArray();
	}
    //inserts data from one sensor into the buffer
    //Display.insertSensorValuesToBuffer(inDataArray[2],inDataArray[3],inDataArray[4],inDataArray[5]);
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
    //Display.drawSensorNames();
    
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
		if(Firefly.getMap){
		Bus.requestRow(Firefly.mapNumber);
		Firefly.getMap = false; 
		}
			
		//Display.draw(0x80,0x53);
	}	
}