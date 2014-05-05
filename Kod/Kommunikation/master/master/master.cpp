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

int LED = 0;
bool LEDtoggle = true;

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
			Firefly.getMap = false;
			Firefly.mapNumber = 0;
		}else{
			Firefly.mapNumber++;
			Firefly.getMap = true;
		}
	}
	if(Firefly.outDataArray[1]=='g'){
		Bus.outDataArray[0] = 2;
		Bus.outDataArray[1] = 'g';
		Bus.outDataArray[2] = 1;
		Bus.sendArray(0);	
	}
}	

void handleDataFromSensor(){
	ReceiveFromSensor=false;
	memcpy(Bus.buffer, Bus.inDataArray,27);
	
	if(Bus.buffer[1] == 'S'){
		// copy data to Bus outDataArray
		memcpy(Bus.outDataArray, Bus.buffer,27);
		Bus.sendArray(1);
		memcpy(Firefly.outDataArray, Bus.buffer,27);
		Firefly.sendArray();
		//inserts data from all sensors into the Display-buffer
		if (Display.bufferWritten)
		{
			Display.insertSensorValuesToBuffer(Bus.buffer);
			Display.bufferWritten = false;
		}
	}
	if(Bus.buffer[1] == 'G'){
		memcpy(Bus.outDataArray, Bus.buffer,2);
		Bus.sendArray(1);
	}
	if(Bus.buffer[1] == 'R'){
		Bus.outDataArray[0] = 1;
		Bus.outDataArray[1] = 'r';
		Bus.sendArray(0);
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

//Steer module wants to send data
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
	DDRA |= (1<<PORTA4);
    Firefly.setPointer(&Bus,&buffer);
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
		Display.update();
		
		if(LED = 500){
			LED = 0;
			if(LEDtoggle){
				PORTA |=(1<<PORTA4);
				LEDtoggle = false;
			}
			else{
				PORTA &= ~(1<<PORTA4);
				LEDtoggle = true;
			}
		}
		LED++;
	}
}